/* ====================================================================
 * Copyright (c) 2005-2007 Open Source Applications Foundation.
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
#include <datetime.h>

#include <unicode/ustring.h>

#include "bases.h"

static PyObject *utcoffset_NAME;
static PyObject *toordinal_NAME;


typedef struct {
    UConverterCallbackReason reason;
    char chars[8];
    int32_t length;
} _STOPReason;

U_STABLE void U_EXPORT2 _stopDecode(const void *context,
                                    UConverterToUnicodeArgs *args,
                                    const char *chars, int32_t length,
                                    UConverterCallbackReason reason,
                                    UErrorCode *err)
{
    _STOPReason *stop = (_STOPReason *) context;
    int len = length < (int)sizeof(stop->chars)-1 ? length : sizeof(stop->chars)-1;

    stop->reason = reason;
    if (chars && len)
        strncpy(stop->chars, chars, len); stop->chars[len] = '\0';
    stop->length = length;
}


PyObject *PyExc_ICUError;
PyObject *PyExc_InvalidArgsError;


EXPORT ICUException::ICUException()
{
    code = NULL;
    msg = NULL;
}

EXPORT ICUException::ICUException(UErrorCode status)
{
    PyObject *messages = PyObject_GetAttrString(PyExc_ICUError, "messages");

    code = PyInt_FromLong((long) status);
    msg = PyObject_GetItem(messages, code);
    Py_DECREF(messages);
}

EXPORT ICUException::ICUException(UErrorCode status, char *format, ...)
{
    ICUException::code = PyInt_FromLong((long) status);

    va_list ap;
    va_start(ap, format);
    ICUException::msg = PyString_FromFormatV(format, ap);
    va_end(ap);
}

EXPORT ICUException::ICUException(UParseError &pe, UErrorCode status)
{
    PyObject *messages = PyObject_GetAttrString(PyExc_ICUError, "messages");
    icu::UnicodeString pre((const UChar *) pe.preContext, U_PARSE_CONTEXT_LEN);
    icu::UnicodeString post((const UChar *) pe.postContext, U_PARSE_CONTEXT_LEN);
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

EXPORT ICUException::~ICUException()
{
    Py_XDECREF(ICUException::code);
    Py_XDECREF(ICUException::msg);
}

EXPORT PyObject *ICUException::reportError()
{
    if (ICUException::code)
    {
        PyObject *tuple = Py_BuildValue("(OO)", ICUException::code, ICUException::msg ? ICUException::msg : Py_None);

        PyErr_SetObject(PyExc_ICUError, tuple);
        Py_DECREF(tuple);
    }
        
    return NULL;
}


EXPORT PyObject *PyUnicode_FromUnicodeString(icu::UnicodeString *string)
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
        int len = string->length();
        PyObject *u = PyUnicode_FromUnicode(NULL, len);

        if (u)
        {
            Py_UNICODE *pchars = PyUnicode_AS_UNICODE(u);
            const UChar *chars = string->getBuffer();

            for (int i = 0; i < len; i++)
                pchars[i] = chars[i];
        }        

        return u;
    }
}

EXPORT PyObject *PyUnicode_FromUnicodeString(const UChar *chars, int size)
{
    if (!chars)
    {
        Py_INCREF(Py_None);
        return Py_None;
    }
    else if (sizeof(Py_UNICODE) == sizeof(UChar))
        return PyUnicode_FromUnicode((const Py_UNICODE *) chars, size);
    else
    {
        PyObject *u = PyUnicode_FromUnicode(NULL, size);

        if (u)
        {
            Py_UNICODE *pchars = PyUnicode_AS_UNICODE(u);

            for (int i = 0; i < size; i++)
                pchars[i] = chars[i];
        }
        
        return u;
    }
}

EXPORT icu::UnicodeString &PyString_AsUnicodeString(PyObject *object,
                                                    char *encoding, char *mode,
                                                    icu::UnicodeString &string)
{
    UErrorCode status = U_ZERO_ERROR;
    UConverter *conv = ucnv_open(encoding, &status);
    icu::UnicodeString result;

    if (U_FAILURE(status))
        throw ICUException(status);

    _STOPReason stop;
    char *src;
    Py_ssize_t len;

    memset(&stop, 0, sizeof(stop));

    if (!strcmp(mode, "strict"))
    {
        ucnv_setToUCallBack(conv, _stopDecode, &stop, NULL, NULL, &status);
        if (U_FAILURE(status))
            throw ICUException(status);
    }

    PyString_AsStringAndSize(object, &src, &len);
    result = icu::UnicodeString((const char *) src, (int32_t) len, conv, status);

    if (U_FAILURE(status))
    {
        char *reasonName;

        switch (stop.reason) {
          case UCNV_UNASSIGNED:
            reasonName = "the code point is unassigned";
            break;
          case UCNV_ILLEGAL:
            reasonName = "the code point is illegal";
            break;
          case UCNV_IRREGULAR:
            reasonName = "the code point is not a regular sequence in the encoding";
            break;
          default:
            reasonName = "unexpected";
            break;
        }
        status = U_ZERO_ERROR;

        int position = strstr(src, stop.chars) - src;
        PyObject *msg = PyString_FromFormat("'%s' codec can't decode byte 0x%x in position %d: %d (%s)", ucnv_getName(conv, &status), (int) (unsigned char) stop.chars[0], position, stop.reason, reasonName);

        PyErr_SetObject(PyExc_ValueError, msg);
        Py_DECREF(msg);
        ucnv_close(conv);

        throw ICUException();
    }

    ucnv_close(conv);
    string.setTo(result);

    return string;
}

EXPORT icu::UnicodeString &PyObject_AsUnicodeString(PyObject *object,
                                                    char *encoding, char *mode,
                                                    icu::UnicodeString &string)
{
    if (PyUnicode_CheckExact(object))
    {
        if (sizeof(Py_UNICODE) == sizeof(UChar))
            string.setTo((const UChar *) PyUnicode_AS_UNICODE(object),
                         (int32_t) PyUnicode_GET_SIZE(object));
        else
        {
            int32_t len = (int32_t) PyUnicode_GET_SIZE(object);
            Py_UNICODE *pchars = PyUnicode_AS_UNICODE(object);
            UChar *chars = new UChar[len * 3];
            UErrorCode status = U_ZERO_ERROR;
            int32_t dstLen;

            u_strFromUTF32(chars, len * 3, &dstLen,
                           (const UChar32 *) pchars, len, &status);

            if (U_FAILURE(status))
            {
                delete chars;
                throw ICUException(status);
            }

            string.setTo((const UChar *) chars, (int32_t) dstLen);
            delete chars;
        }
    }
    else if (PyString_CheckExact(object))
        PyString_AsUnicodeString(object, encoding, mode, string);
    else
    {
        PyErr_SetObject(PyExc_TypeError, object);
        throw ICUException();
    }

    return string;
}

EXPORT icu::UnicodeString &PyObject_AsUnicodeString(PyObject *object,
                                                    icu::UnicodeString &string)
{
    return PyObject_AsUnicodeString(object, "utf-8", "strict", string);
}

EXPORT icu::UnicodeString *PyObject_AsUnicodeString(PyObject *object)
{
    if (object == Py_None)
        return NULL;
    else
    {
        icu::UnicodeString string;

        try {
            PyObject_AsUnicodeString(object, string);
        } catch (ICUException e) {
            throw e;
        }

        return new icu::UnicodeString(string);
    }
}


#if PY_VERSION_HEX < 0x02040000
    /* Replace some _CheckExact macros for Python < 2.4 since the actual
     * datetime types are private until then.  This is ugly, but allows
     * support for datetime objects in Python 2.3.
     */
    #include <string.h>

    #undef PyDateTime_CheckExact
    #define PyDateTime_CheckExact(op) \
       (!strcmp((op)->ob_type->tp_name, "datetime.datetime"))

    #undef PyDelta_CheckExact
    #define PyDelta_CheckExact(op) \
       (!strcmp((op)->ob_type->tp_name, "datetime.timedelta"))
#endif


int isDate(PyObject *object)
{
    if (PyFloat_CheckExact(object))
        return 1;

    return PyDateTime_CheckExact(object);
}

int isDateExact(PyObject *object)
{
    return PyDateTime_CheckExact(object);
}

EXPORT UDate PyObject_AsUDate(PyObject *object)
{
    if (PyFloat_CheckExact(object))
        return (UDate) (PyFloat_AsDouble(object) * 1000.0);
    else
    {
        if (PyDateTime_CheckExact(object))
        {
            PyObject *tzinfo = PyObject_GetAttrString(object, "tzinfo");
            PyObject *utcoffset, *ordinal;

            if (tzinfo == Py_None)
            {
                PyObject *m = PyImport_ImportModule("PyICU");
                PyObject *cls = PyObject_GetAttrString(m, "ICUtzinfo");

                tzinfo = PyObject_GetAttrString(cls, "default");
                Py_DECREF(cls);
                Py_DECREF(m);

                utcoffset = PyObject_CallMethodObjArgs(tzinfo, utcoffset_NAME,
                                                       object, NULL);
                Py_DECREF(tzinfo);
            }
            else
            {
                utcoffset = PyObject_CallMethodObjArgs(object, utcoffset_NAME,
                                                       NULL);
                Py_DECREF(tzinfo);
            }

            ordinal = PyObject_CallMethodObjArgs(object, toordinal_NAME, NULL);

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
    
    PyErr_SetObject(PyExc_TypeError, object);
    throw ICUException();
}

int abstract_init(PyObject *self, PyObject *args, PyObject *kwds)
{
    PyObject *err = Py_BuildValue("(sO)", "instantiating class", self->ob_type);

    PyErr_SetObject(PyExc_NotImplementedError, err);
    Py_DECREF(err);

    return -1;
}

static PyObject *types;

void registerType(PyTypeObject *type, UClassID id)
{
    PyObject *n = PyInt_FromLong((long) id);
    PyObject *list = PyList_New(0);
    PyObject *bn;

    PyDict_SetItem(types, n, list); Py_DECREF(list);
    PyDict_SetItem(types, (PyObject *) type, n);

    while (type != &UObjectType) {
        type = type->tp_base;
        bn = PyDict_GetItem(types, (PyObject *) type);
        list = PyDict_GetItem(types, bn);
        PyList_Append(list, n);
    }

    Py_DECREF(n);
}

int isInstance(PyObject *arg, UClassID id, PyTypeObject *type)
{
    if (PyObject_TypeCheck(arg, &UObjectType))
    {
        UClassID oid = ((t_uobject *) arg)->object->getDynamicClassID();

        if (id == oid)
            return 1;

        PyObject *bn = PyInt_FromLong((long) id);
        PyObject *n = PyInt_FromLong((long) oid);
        PyObject *list = PyDict_GetItem(types, bn);
        int b = PySequence_Contains(list, n);
        
        Py_DECREF(bn);
        Py_DECREF(n);

        return b ? b : PyObject_TypeCheck(arg, type);
    }

    return 0;
}

icu::UObject **pl2cpa(PyObject *arg, int *len, UClassID id, PyTypeObject *type)
{
    if (PySequence_Check(arg))
    {
        *len = PySequence_Size(arg);
        icu::UObject **array = (icu::UObject **)
            calloc(*len, sizeof(icu::UObject *));

        for (int i = 0; i < *len; i++) {
            PyObject *obj = PySequence_GetItem(arg, i);

            if (isInstance(obj, id, type))
            {
                array[i] = ((t_uobject *) obj)->object;
                Py_DECREF(obj);
            }
            else
            {
                Py_DECREF(obj);
                free(array);
                return NULL;
            }
        }

        return array;
    }
    
    return NULL;
}

PyObject *cpa2pl(icu::UObject **array, int len,
                PyObject *(*wrap)(icu::UObject *, int))
{
    PyObject *list = PyList_New(len);

    for (int i = 0; i < len; i++)
        PyList_SET_ITEM(list, i, wrap(array[i], T_OWNED));

    return list;
}

icu::Formattable *toFormattable(PyObject *arg)
{
    UDate date;
    double d;
    int i;
    PY_LONG_LONG l;
    icu::UnicodeString *u;
    icu::UnicodeString _u;
    char *s;

    if (!parseArg(arg, "d", &d))
        return new icu::Formattable(d);

    if (!parseArg(arg, "i", &i))
        return new icu::Formattable(i);

    if (!parseArg(arg, "L", &l))
        return new icu::Formattable(l);

    if (!parseArg(arg, "c", &s))
        return new icu::Formattable(s);

    if (!parseArg(arg, "S", &u, &_u))
        return new icu::Formattable(*u);

    if (!parseArg(arg, "E", &date))
        return new icu::Formattable(date, icu::Formattable::kIsDate);

    return NULL;
}

icu::Formattable *toFormattableArray(PyObject *arg, int *len,
                                     UClassID id, PyTypeObject *type)
{
    if (PySequence_Check(arg))
    {
        *len = PySequence_Size(arg);
        icu::Formattable *array = new icu::Formattable[*len + 1];

        for (int i = 0; i < *len; i++) {
            PyObject *obj = PySequence_GetItem(arg, i);

            if (isInstance(obj, id, type))
            {
                array[i] = *(icu::Formattable *) ((t_uobject *) obj)->object;
                Py_DECREF(obj);
            }
            else
            {
                icu::Formattable *f = toFormattable(obj);

                if (f)
                {
                    array[i] = *f;
                    delete f;
                    Py_DECREF(obj);
                }
                else
                {
                    Py_DECREF(obj);
                    delete[] array;
                    return NULL;
                }
            }
        }

        return array;
    }

    return NULL;
}

static icu::UnicodeString *toUnicodeStringArray(PyObject *arg, int *len)
{
    if (PySequence_Check(arg))
    {
        *len = PySequence_Size(arg);
        icu::UnicodeString *array = new icu::UnicodeString[*len + 1];

        for (int i = 0; i < *len; i++) {
            PyObject *obj = PySequence_GetItem(arg, i);
            
            if (PyObject_TypeCheck(obj, &UObjectType))
            {
                array[i] = *(icu::UnicodeString *) ((t_uobject *) obj)->object;
                Py_DECREF(obj);
            }
            else
            {
                try {
                    PyObject_AsUnicodeString(obj, array[i]);
                } catch (ICUException e) {
                    Py_DECREF(obj);
                    e.reportError();
                    delete[] array;
                    return NULL;
                }
            }
        }

        return array;
    }

    return NULL;
}

static double *toDoubleArray(PyObject *arg, int *len)
{
    if (PySequence_Check(arg))
    {
        *len = PySequence_Size(arg);
        double *array = new double[*len + 1];

        for (int i = 0; i < *len; i++) {
            PyObject *obj = PySequence_GetItem(arg, i);

            if (PyFloat_Check(obj))
            {
                array[i] = PyFloat_AsDouble(obj);
                Py_DECREF(obj);
            }
            else if (PyInt_Check(obj))
            {
                array[i] = (double) PyInt_AsLong(obj);
                Py_DECREF(obj);
            }
            else if (PyLong_Check(obj))
            {
                array[i] = PyLong_AsDouble(obj);
                Py_DECREF(obj);
            }
            else
            {
                Py_DECREF(obj);
                delete[] array;
                return NULL;
            }
        }

        return array;
    }

    return NULL;
}

static UBool *toUBoolArray(PyObject *arg, int *len)
{
    if (PySequence_Check(arg))
    {
        *len = PySequence_Size(arg);
        UBool *array = new UBool[*len + 1];

        for (int i = 0; i < *len; i++) {
            PyObject *obj = PySequence_GetItem(arg, i);

            array[i] = (UBool) PyObject_IsTrue(obj);
            Py_DECREF(obj);
        }

        return array;
    }

    return NULL;
}

#ifdef _MSC_VER

int __parseArgs(PyObject *args, char *types, ...)
{
    int count = ((PyTupleObject *)(args))->ob_size;
    va_list list;

    va_start(list, types);

    return _parseArgs(((PyTupleObject *)(args))->ob_item, count, types, list);
}

int __parseArg(PyObject *arg, char *types, ...)
{
    va_list list;

    va_start(list, types);

    return _parseArgs(&arg, 1, types, list);
}


int _parseArgs(PyObject **args, int count, char *types, va_list list)
{
    if (count != strlen(types))
        return -1;

#else

int _parseArgs(PyObject **args, int count, char *types, ...)
{
    va_list list;

    if (count != (int)strlen(types))
        return -1;

    va_start(list, types);

#endif

    for (int i = 0; i < count; i++) {
        PyObject *arg = args[i];
        
        switch (types[i]) {
          case 'c':           /* string */
          case 'k':           /* string and size */
          case 'C':           /* string, not to be unpacked */
            if (PyString_Check(arg))
                break;
            return -1;

          case 's':           /* string or unicode */
            if (PyString_Check(arg) || PyUnicode_Check(arg))
                break;
            return -1;

          case 'S':           /* string, unicode or UnicodeString */
            if (PyString_Check(arg) || PyUnicode_Check(arg) ||
                isUnicodeString(arg))
                break;
            return -1;

          case 'T':           /* array of string, unicode or UnicodeString */
            if (PySequence_Check(arg))
            {
                if (PySequence_Length(arg) > 0)
                {
                    PyObject *obj = PySequence_GetItem(arg, 0);
                    int ok = (PyString_Check(obj) || PyUnicode_Check(obj) ||
                              isUnicodeString(obj));
                    Py_DECREF(obj);
                    if (ok)
                        break;
                }
                else
                    break;
            }
            return -1;

          case 'U':           /* UnicodeString */
            if (isUnicodeString(arg))
                break;
            return -1;

          case 'P':           /* wrapped ICU object */
          {
              UClassID id = va_arg(list, UClassID);
              PyTypeObject *type = va_arg(list, PyTypeObject *);

              if (isInstance(arg, id, type))
                  break;
              return -1;
          }

          case 'Q':           /* array of wrapped ICU object pointers */
          case 'R':           /* array of wrapped ICU objects */
          {
              UClassID id = va_arg(list, UClassID);
              PyTypeObject *type = va_arg(list, PyTypeObject *);
              
              if (PySequence_Check(arg))
              {
                  if (PySequence_Length(arg) > 0)
                  {
                      PyObject *obj = PySequence_GetItem(arg, 0);
                      int ok = isInstance(obj, id, type);

                      Py_DECREF(obj);
                      if (ok)
                          break;
                  }
                  else
                      break;
              }
              return -1;
          }

          case 'D':           /* date as UDate float or datetime */
            if (isDate(arg))
                break;
            return -1;

          case 'E':           /* date as datetime */
            if (isDateExact(arg))
                break;
            return -1;

          case 'a':           /* byte */
            if (PyString_Check(arg) && (PyString_Size(arg) == 1))
                break;
            return -1;

          case 'B':           /* boolean, strict */
            if (arg == Py_True || arg == Py_False)
                break;
            return -1;

          case 'b':           /* boolean */
            break;

          case 'i':           /* int */
            if (PyInt_Check(arg))
                break;
            return -1;

          case 'd':           /* double */
            if (PyFloat_Check(arg) || PyInt_Check(arg) || PyLong_Check(arg))
                break;
            return -1;

          case 'F':           /* array of double */
            if (PySequence_Check(arg))
            {
                if (PySequence_Length(arg) > 0)
                {
                    PyObject *obj = PySequence_GetItem(arg, 0);
                    int ok = (PyFloat_Check(obj) ||
                              PyInt_Check(obj) ||
                              PyLong_Check(obj));
                    Py_DECREF(obj);
                    if (ok)
                        break;
                }
                else
                    break;
            }
            return -1;

          case 'G':           /* array of bool */
            if (PySequence_Check(arg))
                break;
            return -1;

          case 'L':           /* PY_LONG_LONG */
            if (PyLong_Check(arg) || PyInt_Check(arg))
                break;
            return -1;

          default:
            return -1;
        }
    }

    for (int j = 0; j < count; j++) {
        PyObject *arg = args[j];
        
        switch (types[j]) {
          case 'c':           /* string */
          {
              char **c = va_arg(list, char **);
              *c = PyString_AS_STRING(arg);
              break;
          }

          case 'k':           /* string and size */
          {
              char **c = va_arg(list, char **);
              int *l = va_arg(list, int *);
              *c = PyString_AS_STRING(arg);
              *l = PyString_GET_SIZE(arg);
              break;
          }

          case 'C':           /* string, not to be unpacked */
          {
              PyObject **obj = va_arg(list, PyObject **);
              *obj = arg;
              break;
          }

          case 's':           /* string or unicode  */
          {
              icu::UnicodeString **u = va_arg(list, icu::UnicodeString **);
              try {
                  *u = PyObject_AsUnicodeString(arg);
              } catch (ICUException e) {
                  e.reportError();
                  return -1;
              }
              break;
          }

          case 'S':           /* string, unicode or UnicodeString */
          {
              icu::UnicodeString **u = va_arg(list, icu::UnicodeString **);
              icu::UnicodeString *_u = va_arg(list, icu::UnicodeString *);
              if (PyObject_TypeCheck(arg, &UObjectType))
                  *u = (icu::UnicodeString *) ((t_uobject *) arg)->object;
              else
              {
                  try {
                      PyObject_AsUnicodeString(arg, *_u);
                      *u = _u;
                  } catch (ICUException e) {
                      e.reportError();
                      return -1;
                  }
              }
              break;
          }

          case 'T':           /* array of string, unicode or UnicodeString */
          {
              icu::UnicodeString **array = va_arg(list, icu::UnicodeString **);
              int *len = va_arg(list, int *);
              *array = toUnicodeStringArray(arg, len);
              if (!*array)
                  return -1;
              break;
          }

          case 'U':           /* UnicodeString */
          {
              icu::UnicodeString **u = va_arg(list, icu::UnicodeString **);
              *u = (icu::UnicodeString *) ((t_uobject *) arg)->object;
              break;
          }

          case 'P':           /* wrapped ICU object */
          {
              icu::UObject **obj = va_arg(list, icu::UObject **);
              *obj = ((t_uobject *) arg)->object;
              break;
          }

          case 'Q':           /* array of wrapped ICU object pointers */
          {
              icu::UObject ***array = va_arg(list, icu::UObject ***);
              int *len = va_arg(list, int *);
              UClassID id = va_arg(list, UClassID);
              PyTypeObject *type = va_arg(list, PyTypeObject *);
              *array = pl2cpa(arg, len, id, type);
              if (!*array)
                  return -1;
              break;
          }

          case 'R':           /* array of wrapped ICU objects */
          {
	      typedef icu::UObject *(*convFn)(PyObject *, int *,
					      UClassID, PyTypeObject *);
              icu::UObject **array = va_arg(list, icu::UObject **);
              int *len = va_arg(list, int *);
              UClassID id = va_arg(list, UClassID);
              PyTypeObject *type = va_arg(list, PyTypeObject *);
	      convFn fn = va_arg(list, convFn);
              *array = fn(arg, len, id, type);
              if (!*array)
                  return -1;
              break;
          }

          case 'D':           /* date as UDate float or datetime */
          case 'E':           /* date as datetime */
          {
              UDate *d = va_arg(list, UDate *);
              *d = PyObject_AsUDate(arg);
              break;
          }

          case 'a':           /* byte */
          {
              unsigned char *a = va_arg(list, unsigned char *);
              *a = (unsigned char) PyString_AS_STRING(arg)[0];
              break;
          }

          case 'B':           /* boolean, strict */
          case 'b':           /* boolean */
          {
              int *b = va_arg(list, int *);
              *b = PyObject_IsTrue(arg);
              break;
          }

          case 'i':           /* int */
          {
              int *n = va_arg(list, int *);
              *n = PyInt_AsLong(arg);
              break;
          }

          case 'd':           /* double */
          {
              double *d = va_arg(list, double *);
              if (PyFloat_Check(arg))
                  *d = PyFloat_AsDouble(arg);
              else if (PyInt_Check(arg))
                  *d = (double) PyInt_AsLong(arg);
              else
                  *d = PyLong_AsDouble(arg);
              break;
          }

          case 'F':           /* array of double */
          {
              double **array = va_arg(list, double **);
              int *len = va_arg(list, int *);
              *array = toDoubleArray(arg, len);
              if (!*array)
                  return -1;
              break;
          }

          case 'G':           /* array of UBool */
          {
              UBool **array = va_arg(list, UBool **);
              int *len = va_arg(list, int *);
              *array = toUBoolArray(arg, len);
              if (!*array)
                  return -1;
              break;
          }

          case 'L':           /* PY_LONG_LONG */
          {
              PY_LONG_LONG *l = va_arg(list, PY_LONG_LONG *);
              *l = PyLong_AsLongLong(arg);
              break;
          }

          default:
            return -1;
        }
    }

    return 0;
}

PyObject *PyErr_SetArgsError(PyObject *self, char *name, PyObject *args)
{
    if (!PyErr_Occurred())
    {
        PyObject *type = (PyObject *) self->ob_type;
        PyObject *err = Py_BuildValue("(OsO)", type, name, args);

        PyErr_SetObject(PyExc_InvalidArgsError, err);
        Py_DECREF(err);
    }

    return NULL;
}

PyObject *PyErr_SetArgsError(PyTypeObject *type, char *name, PyObject *args)
{
    if (!PyErr_Occurred())
    {
        PyObject *err = Py_BuildValue("(OsO)", type, name, args);

        PyErr_SetObject(PyExc_InvalidArgsError, err);
        Py_DECREF(err);
    }

    return NULL;
}

int isUnicodeString(PyObject *arg)
{
    return (PyObject_TypeCheck(arg, &UObjectType) &&
            (((t_uobject *) arg)->object->getDynamicClassID() ==
             icu::UnicodeString::getStaticClassID()));
}

void _init_common(PyObject *m)
{
    types = PyDict_New();
    PyModule_AddObject(m, "__types__", types);

#if PY_VERSION_HEX > 0x02040000
    PyDateTime_IMPORT;
#endif

    utcoffset_NAME = PyString_FromString("utcoffset");
    toordinal_NAME = PyString_FromString("toordinal");
}
