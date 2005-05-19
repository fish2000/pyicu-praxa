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


%{

#include "common.h"

%}


%typemap(out) PyObject *
{
    $result = $1;
}

%typemap(in) UBool
{
    $1 = $input == Py_True;
}
%typemap(out) UBool
{
    $result = PyBool_FromLong((long) $1);
}
%typecheck(SWIG_TYPECHECK_BOOL) UBool
{
    $1 = $input == Py_True || $input == Py_False;
}

%typemap(in) UChar
{
    if (PyUnicode_CheckExact($input) && PyUnicode_GetSize($input) == 1)
        $1 = (UChar) PyUnicode_AsUnicode($input)[0];
    else if (PyString_CheckExact($input) && PyString_Size($input) == 1)
        $1 = (UChar) PyString_AsString($input)[0];
    else
    {
        PyErr_SetObject(PyExc_ValueError, $input);
        SWIG_fail;
    }
}
%typemap(out) UChar
{
    Py_UNICODE u = (Py_UNICODE) $1;
    $result = PyUnicode_FromUnicode(&u, 1);
}
%typecheck(SWIG_TYPECHECK_INT16) UChar
{
    $1 = (PyUnicode_CheckExact($input) && PyUnicode_GetSize($input) == 1 ||
          PyString_CheckExact($input) && PyString_Size($input) == 1);
}

%typemap(in) ISO3Code (UnicodeString u)
{
    try {
        PyUnicode_AsUnicodeString($input, u);
    } catch (ICUException e) {
        SWIG_fail;
    }
    $1 = u.getTerminatedBuffer();
}
%typemap(out) ISO3Code
{
    UnicodeString u($1);
    $result = PyUnicode_FromUnicodeString(&u);
}
%typecheck(SWIG_TYPECHECK_STRING) ISO3Code
{
    $1 = (PyUnicode_CheckExact($input) && PyUnicode_GetSize($input) == 3 ||
          PyString_CheckExact($input) && PyString_Size($input) == 3);
}

%typemap(in) int8_t
{
    $1 = (int8_t) PyInt_AsLong($input);
}
%typemap(out) int8_t
{
    $result = PyInt_FromLong((long) $1);
}
%typecheck(SWIG_TYPECHECK_INT8) int8_t
{
    $1 = PyInt_CheckExact($input);
}

%typemap(in) uint8_t
{
    $1 = (uint8_t) PyInt_AsLong($input);
}
%typemap(out) uint8_t
{
    $result = PyInt_FromLong((long) $1);
}
%typecheck(SWIG_TYPECHECK_UINT8) uint8_t
{
    $1 = PyInt_CheckExact($input);
}

%typemap(in) int32_t
{
    $1 = (int32_t) PyInt_AsLong($input);
}
%typemap(out) int32_t
{
    $result = PyInt_FromLong((long) $1);
}
%typecheck(SWIG_TYPECHECK_INT32) int32_t
{
    $1 = PyInt_CheckExact($input);
}

%typemap(in) int64_t
{
    $1 = (int64_t) PyInt_AsLong($input);
}
%typemap(out) int64_t
{
    $result = PyInt_FromLong((long) $1);
}
%typecheck(SWIG_TYPECHECK_INT64) int64_t
{
    $1 = PyLong_CheckExact($input);
}

%typemap(in) double {

    $1 = (double) PyFloat_AsDouble($input);
}
%typemap(out) double {

    $result = PyFloat_FromDouble((double) $1);
}
%typecheck(SWIG_TYPECHECK_DOUBLE) double {

    $1 = PyFloat_CheckExact($input);
}

%typemap(in) UDate
{
    $1 = PyObject_AsUDate($input);
}
%typemap(out) UDate
{
    $result = PyFloat_FromDouble((double) $1 / 1000.0);
}
%typecheck(SWIG_TYPECHECK_DOUBLE) UDate
{
    $1 = (PyFloat_CheckExact($input) ||
          PyObject_HasAttrString($input, "timetuple"));
}

%typemap(in, numinputs=0) _UnicodeString
{
}

%typemap(in, numinputs=0) _int32_t
{
}

%typemap(out) UnicodeString0 &
{
    Py_INCREF(obj0);
    $result = obj0;
}
%typemap(out) UnicodeString1 &
{
    Py_INCREF(obj1);
    $result = obj1;
}
%typemap(out) UnicodeString2 &
{
    Py_INCREF(obj2);
    $result = obj2;
}
%typemap(out) UnicodeString3 &
{
    Py_INCREF(obj3);
    $result = obj3;
}

%typemap(out) Formattable2 &
{
    Py_INCREF(obj2);
    $result = obj2;
}

%typemap(out) _UnicodeString *
{
    $result = SWIG_NewPointerObj($1, $descriptor(icu::UnicodeString *), 1);
}

%typemap(out) _NumberFormat *
{
    if ($1->getDynamicClassID() == icu::DecimalFormat::getStaticClassID())
        $result = SWIG_NewPointerObj($1, $descriptor(icu::DecimalFormat *), 1);
    else
        $result = SWIG_NewPointerObj($1, $descriptor(icu::NumberFormat *), 1);
}

%typemap(out) _DateFormat *
{
    if ($1->getDynamicClassID() == icu::SimpleDateFormat::getStaticClassID())
        $result = SWIG_NewPointerObj($1, $descriptor(icu::SimpleDateFormat *), 1);
    else
        $result = SWIG_NewPointerObj($1, $descriptor(icu::DateFormat *), 1);
}

%typemap(out) _Calendar *
{
    if ($1->getDynamicClassID() == icu::GregorianCalendar::getStaticClassID())
        $result = SWIG_NewPointerObj($1, $descriptor(icu::GregorianCalendar *), 1);
    else
        $result = SWIG_NewPointerObj($1, $descriptor(icu::Calendar *), 1);
}

%typemap(out) _TimeZone *
{
    if ($1->getDynamicClassID() == icu::SimpleTimeZone::getStaticClassID())
        $result = SWIG_NewPointerObj($1, $descriptor(icu::SimpleTimeZone *), 1);
    else
        $result = SWIG_NewPointerObj($1, $descriptor(icu::TimeZone *), 1);
}

%typemap(out) const_TimeZone &
{
    if ($1->getDynamicClassID() == icu::SimpleTimeZone::getStaticClassID())
        $result = SWIG_NewPointerObj((void *) $1, $descriptor(icu::SimpleTimeZone *), 0);
    else
        $result = SWIG_NewPointerObj((void *) $1, $descriptor(icu::TimeZone *), 0);
}

%typemap(out) _MeasureFormat *
{
    $result = SWIG_NewPointerObj($1, $descriptor(icu::MeasureFormat *), 1);
}

%typemap(in) _PyString
{
    try {
        PyUnicode_AsUnicodeString($input, $1);
    } catch (ICUException e) {
        e.reportError();
        SWIG_fail;
    }
}
%typemap(out) _PyString
{
    $result = PyUnicode_FromUnicodeString(&$1);
}
%typecheck(SWIG_TYPECHECK_POINTER) _PyString
{
    $1 = $input != Py_None;
}

%typemap(out) _PyString *
{
    $result = PyUnicode_FromUnicodeString($1);
}

%typemap(in, numinputs=0) UErrorCode
{
    $1 = U_ZERO_ERROR;
}
%typemap(argout) UErrorCode
{
    if (U_FAILURE($1))
        return ICUException($1).reportError();
}

%typemap(in, numinputs=0) (UParseError, UErrorCode)
{
    $2 = U_ZERO_ERROR;
}
%typemap(argout) (UParseError, UErrorCode)
{
    if (U_FAILURE($2))
        return ICUException($1, $2).reportError();
}

%typemap(out) LocaleArray1 {

    $result = PyList_New(arg1);
    for (int32_t i = 0; i < arg1; i++) {
        Locale *locale = (Locale *) $1 + i;
        PyObject *o = SWIG_NewPointerObj(locale, $descriptor(icu::Locale *), 0);
        PyList_SET_ITEM($result, i, o);
    }
}


%exception
{
    try {
        $action
    } catch (ICUException e) {
        return e.reportError();
    }
}
