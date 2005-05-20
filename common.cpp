/* ====================================================================
 * Copyright (c) 2005 Open Source Applications Foundation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software. 
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * ====================================================================
 */

#include "common.h"
#include <stdarg.h>
#include <unicode/ucnv.h>
#include <datetime.h>

PyObject *PyExc_ICUError;


ICUException::ICUException()
{
    code = NULL;
    msg = NULL;
}

ICUException::ICUException(UErrorCode status)
{
    PyObject *messages = PyObject_GetAttrString(PyExc_ICUError, "messages");

    code = PyInt_FromLong((long) status);
    msg = PyObject_GetItem(messages, code);
    Py_DECREF(messages);
}

ICUException::ICUException(UErrorCode status, char *format, ...)
{
    ICUException::code = PyInt_FromLong((long) status);

    va_list ap;
    va_start(ap, format);
    ICUException::msg = PyString_FromFormatV(format, ap);
    va_end(ap);
}

ICUException::ICUException(UParseError &pe, UErrorCode status)
{
    PyObject *messages = PyObject_GetAttrString(PyExc_ICUError, "messages");
    UnicodeString pre((const UChar *) pe.preContext, U_PARSE_CONTEXT_LEN);
    UnicodeString post((const UChar *) pe.postContext, U_PARSE_CONTEXT_LEN);
    PyObject *tuple = PyTuple_New(5);

    ICUException::code = PyInt_FromLong((long) status);
    
    PyTuple_SET_ITEM(tuple, 0, PyObject_GetItem(messages, code));
    PyTuple_SET_ITEM(tuple, 1, PyInt_FromLong(pe.line));
    PyTuple_SET_ITEM(tuple, 2, PyInt_FromLong(pe.offset));
    PyTuple_SET_ITEM(tuple, 3, PyUnicode_FromUnicodeString(&pre));
    PyTuple_SET_ITEM(tuple, 4, PyUnicode_FromUnicodeString(&post));
    ICUException::msg = tuple;

    Py_DECREF(messages);
}

ICUException::~ICUException()
{
    Py_XDECREF(ICUException::code);
    Py_XDECREF(ICUException::msg);
}

PyObject *ICUException::reportError()
{
    if (ICUException::code)
    {
        PyObject *tuple = Py_BuildValue("(OO)", ICUException::code, ICUException::msg ? ICUException::msg : Py_None);

        PyErr_SetObject(PyExc_ICUError, tuple);
        Py_DECREF(tuple);
    }
        
    return NULL;
}


PyObject *PyUnicode_FromUnicodeString(UnicodeString *string)
{
    if (!string)
    {
        Py_INCREF(Py_None);
        return Py_None;
    }
    else if (sizeof(Py_UNICODE) == sizeof(UChar))
        return PyUnicode_FromUnicode((const Py_UNICODE *) string->getBuffer(),
                                     (int) string->length());
    else
    {
        int len = (int) string->length();
        Py_UNICODE pchars[len];
        const UChar *chars = string->getBuffer();

        for (int i = 0; i < len; i++)
            pchars[i] = chars[i];
        
        return PyUnicode_FromUnicode((const Py_UNICODE *) pchars, len);
    }
}

UnicodeString &PyUnicode_AsUnicodeString(PyObject *object,
                                         UnicodeString &string)
{
    if (PyUnicode_CheckExact(object))
    {
        if (sizeof(Py_UNICODE) == sizeof(UChar))
            string.setTo((const UChar *) PyUnicode_AS_UNICODE(object),
                         (int32_t) PyUnicode_GET_SIZE(object));
        else
        {
            int len = PyUnicode_GET_SIZE(object);
            Py_UNICODE *pchars = PyUnicode_AS_UNICODE(object);
            UChar chars[len];

            for (int i = 0; i < len; i++)
                chars[i] = pchars[i];

            string.setTo((const UChar *) chars, (int32_t) len);
        }
    }
    else if (PyString_CheckExact(object))
    {
        UErrorCode status = U_ZERO_ERROR;
        UConverter *conv = ucnv_open("utf-8", &status);

        if (U_FAILURE(status))
            throw ICUException(status);

        char *src;
        int len;

        PyString_AsStringAndSize(object, &src, &len);
        string.setTo(UnicodeString((const char *) src, (int32_t) len,
                                   conv, status));
        ucnv_close(conv);

        if (U_FAILURE(status))
            throw ICUException(status, "python str is not unicode or utf-8 encoded: %s", src);
    }
    else
    {
        PyErr_SetObject(PyExc_TypeError, object);
        throw ICUException();
    }

    return string;
}

UnicodeString *PyUnicode_AsUnicodeString(PyObject *object)
{
    if (object == Py_None)
        return NULL;
    else
    {
        UnicodeString *string = new UnicodeString();

        try {
            PyUnicode_AsUnicodeString(object, *string);
        } catch (ICUException e) {
            delete string;
            throw e;
        }

        return string;
    }
}


UDate PyObject_AsUDate(PyObject *object)
{
    if (PyFloat_CheckExact(object))
        return (UDate) (PyFloat_AsDouble(object) * 1000.0);
    else
    {
        static PyDateTime_CAPI *PyDateTimeAPI = NULL;
        static PyObject *mktime = NULL;

        if (PyDateTimeAPI == NULL)
            PyDateTimeAPI = (PyDateTime_CAPI *)
                PyCObject_Import("datetime", "datetime_CAPI");
        if (mktime == NULL)
        {
            PyObject *time = PyImport_ImportModule("time");

            mktime = PyObject_GetAttrString(time, "mktime");
            Py_DECREF(time);
        }

        if (PyDateTime_CheckExact(object))
        {
            PyObject *tzinfo = PyObject_GetAttrString(object, "tzinfo");
            PyObject *time = NULL;

            if (tzinfo == Py_None)
            {
                PyObject *method, *args;

                method = PyString_FromString("timetuple");
                args = PyTuple_New(1);

                PyTuple_SET_ITEM(args, 0, PyObject_CallMethodObjArgs(object, method, NULL));
                Py_DECREF(method);

                time = PyObject_Call(mktime, args, NULL);
                Py_DECREF(args);
                Py_DECREF(tzinfo);

                if (time != NULL)
                {
                    if (PyFloat_CheckExact(time))
                    {
                        UDate date = (UDate) (PyFloat_AsDouble(time) * 1000.0);
                        Py_DECREF(time);

                        return date;
                    }

                    Py_DECREF(time);
                }
            }
            else
            {
                PyObject *method, *utcoffset, *ordinal;
                Py_DECREF(tzinfo);

                method = PyString_FromString("utcoffset");
                utcoffset = PyObject_CallMethodObjArgs(object, method, NULL);
                Py_DECREF(method);

                method = PyString_FromString("toordinal");
                ordinal = PyObject_CallMethodObjArgs(object, method, NULL);
                Py_DECREF(method);

                if (utcoffset != NULL && PyDelta_CheckExact(utcoffset) &&
                    ordinal != NULL && PyInt_CheckExact(ordinal))
                {
                    double timestamp =
                        (PyInt_AsLong(ordinal) - 719163) * 86400.0 +
                        PyDateTime_DATE_GET_HOUR(object) * 3600.0 +
                        PyDateTime_DATE_GET_MINUTE(object) * 60.0 +
                        (double) PyDateTime_DATE_GET_SECOND(object) +
                        PyDateTime_DATE_GET_MICROSECOND(object) / 1e6 -
                        (((PyDateTime_Delta *) utcoffset)->days * 86400.0 +
                         (double) ((PyDateTime_Delta *) utcoffset)->seconds);

                    Py_DECREF(utcoffset);
                    Py_DECREF(ordinal);

                    return (UDate) (timestamp * 1000.0);
                }

                Py_XDECREF(utcoffset);
                Py_XDECREF(ordinal);
            }
        }
    }
    
    PyErr_SetObject(PyExc_TypeError, object);
    throw ICUException();
}
