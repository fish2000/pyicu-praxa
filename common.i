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
        PyObject_AsUnicodeString($input, u);
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

%typemap(in) uint32_t
{
    $1 = (int32_t) PyInt_AsLong($input);
}
%typemap(out) uint32_t
{
    $result = (unsigned long) PyInt_FromLong((long) $1);
}
%typecheck(SWIG_TYPECHECK_UINT32) uint32_t
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

%typemap(out) CollationKey2 &
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

%typemap(out) _Collator *
{
    if ($1->getDynamicClassID() == icu::RuleBasedCollator::getStaticClassID())
        $result = SWIG_NewPointerObj($1, $descriptor(icu::RuleBasedCollator *), 1);
    else
        $result = SWIG_NewPointerObj($1, $descriptor(icu::Collator *), 1);
}

%typemap(in) TimeZone_ *
{
    PyObject *thisown = PyObject_GetAttrString($input, "thisown");
    int isTrue = PyObject_IsTrue(thisown); Py_DECREF(thisown);

    if (!isTrue)
    {
        PyErr_Format(PyExc_ValueError,
                     "TimeZone argument is not owned by caller", NULL);
        SWIG_fail;
    }

    if (!SWIG_ConvertPtr($input, (void **) &$1, $descriptor(icu::TimeZone *),
                         SWIG_POINTER_EXCEPTION | SWIG_POINTER_DISOWN))
        PyObject_SetAttrString($input, "this", Py_None);
    else
        SWIG_fail;
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

%typemap(out) _StringEnumeration *
{
    $result = SWIG_NewPointerObj($1, $descriptor(icu::StringEnumeration *), 1);
}

%typemap(in) _PyString
{
    try {
        PyObject_AsUnicodeString($input, $1);
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

%typemap(in, numinputs=1) (UChar *, _int32_t) (UnicodeString u)
{
    try {
        PyObject_AsUnicodeString($input, u);
        $1 = (UChar *) u.getBuffer();
        $2 = u.length();
    } catch (ICUException e) {
        e.reportError();
        SWIG_fail;
    }
}
%typecheck(SWIG_TYPECHECK_POINTER) (UChar *, _int32_t)
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

%typemap(out) LocaleDict1 {

    $result = PyDict_New();
    for (int32_t i = 0; i < arg1; i++) {
        Locale *locale = (Locale *) $1 + i;
        PyObject *o = SWIG_NewPointerObj(locale, $descriptor(icu::Locale *), 0);
        PyDict_SetItemString($result, locale->getName(), o);
	Py_DECREF(o);
    }
}

%typemap(in) UnicodeStringArray3 {

    int len = PyList_Size($input);
    UnicodeString *strings = new UnicodeString[len];
    arg3 = len;
    for (int i = 0; i < len; i++)
        PyObject_AsUnicodeString(PyList_GET_ITEM($input, i), strings[i]);
    $1 = (const UnicodeString *) strings;
}
%typemap(argout) UnicodeStringArray3 {

    delete $1;
}

%typemap(in) UnicodeStringArray4 {

    int len = PyList_Size($input);
    UnicodeString *strings = new UnicodeString[len];
    arg4 = len;
    for (int i = 0; i < len; i++)
        PyObject_AsUnicodeString(PyList_GET_ITEM($input, i), strings[i]);
    $1 = (const UnicodeString *) strings;
}
%typemap(argout) UnicodeStringArray4 {

    delete $1;
}

%typemap(in) UnicodeStringArray5 {

    int len = PyList_Size($input);
    UnicodeString *strings = new UnicodeString[len];
    arg5 = len;
    for (int i = 0; i < len; i++)
        PyObject_AsUnicodeString(PyList_GET_ITEM($input, i), strings[i]);
    $1 = (const UnicodeString *) strings;
}
%typemap(argout) UnicodeStringArray5 {

    delete $1;
}

%typemap(in) LeakyUnicodeStringArray3 {

    int len = PyList_Size($input);
    UnicodeString *strings = new UnicodeString[len]; //leaked
    arg3 = len;
    for (int i = 0; i < len; i++)
        PyObject_AsUnicodeString(PyList_GET_ITEM($input, i), strings[i]);
    $1 = (const UnicodeString *) strings;
}

%typemap(out) UnicodeStringArray2 {

    $result = PyList_New(arg2);
    for (int32_t i = 0; i < arg2; i++) {
        UnicodeString *string = (UnicodeString *) $1 + i;
        PyObject *o = SWIG_NewPointerObj(string, $descriptor(icu::UnicodeString *), 0);
        PyList_SET_ITEM($result, i, o);
    }
}

%typemap(in) FormattableArray3 {

    int len = PyList_Size($input);
    Formattable *array = new Formattable[len];
    arg3 = len;
    for (int i = 0; i < len; i++) {
        PyObject *obj = PyList_GetItem($input, i);
        Formattable *fp;
        if (SWIG_ConvertPtr(obj, (void **) &fp, $descriptor(icu::Formattable *),
                            SWIG_POINTER_EXCEPTION))
        {
            delete array;
            SWIG_fail;
        }
        array[i] = *fp;
    }
    $1 = (const Formattable *) array;
}
%typemap(argout) FormattableArray3 {

    delete $1;
}

%typemap(out) FormattableArray3 {

    $result = PyList_New(arg3);
    for (int32_t i = 0; i < arg3; i++) {
        Formattable *string = (Formattable *) $1 + i;
        PyObject *o = SWIG_NewPointerObj(string, $descriptor(icu::Formattable *), 0);
        PyList_SET_ITEM($result, i, o);
    }
    delete $1;
}

%typemap(out) FormattableArray4 {

    $result = PyList_New(arg4);
    for (int32_t i = 0; i < arg4; i++) {
        Formattable *string = (Formattable *) $1 + i;
        PyObject *o = SWIG_NewPointerObj(string, $descriptor(icu::Formattable *), 0);
        PyList_SET_ITEM($result, i, o);
    }
    delete $1;
}


%typemap(in) FormatPointerArray3 {

    int len = PyList_Size($input);
    Format **formats = new Format *[len];
    arg3 = len;
    for (int i = 0; i < len; i++) {
        PyObject *obj = PyList_GetItem($input, i);
        SWIG_ConvertPtr(obj, (void **)(formats + i), $descriptor(icu::Format *),
                        SWIG_POINTER_EXCEPTION);
    }
    $1 = (const Format **) formats;
}
%typemap(argout) FormatPointerArray3 {

    delete $1;
}

%typemap(out) FormatPointerArray2 {

    $result = PyList_New(arg2);
    for (int32_t i = 0; i < arg2; i++) {
        PyObject *o = SWIG_NewPointerObj((void *) $1[i], $descriptor(icu::Format *), 0);
        PyList_SET_ITEM($result, i, o);
    }
}


%typemap(out) doubleArray2 {

    $result = PyList_New(arg2);
    for (int32_t i = 0; i < arg2; i++)
        PyList_SET_ITEM($result, i, PyFloat_FromDouble($1[i]));
}

%typemap(in) doubleArray3 {

    int len = PyList_Size($input);
    double *doubles = new double[len];
    arg3 = len;
    for (int i = 0; i < len; i++)
        doubles[i] = PyFloat_AsDouble(PyList_GetItem($input, i));
    $1 = (const double *) doubles;
}
%typemap(argout) doubleArray3 {

    delete $1;
}

%typemap(in) doubleArray4 {

    int len = PyList_Size($input);
    double *doubles = new double[len];
    arg4 = len;
    for (int i = 0; i < len; i++)
        doubles[i] = PyFloat_AsDouble(PyList_GetItem($input, i));
    $1 = (const double *) doubles;
}
%typemap(argout) doubleArray4 {

    delete $1;
}

%typemap(in) doubleArray5 {

    int len = PyList_Size($input);
    double *doubles = new double[len];
    arg5 = len;
    for (int i = 0; i < len; i++)
        doubles[i] = PyFloat_AsDouble(PyList_GetItem($input, i));
    $1 = (const double *) doubles;
}
%typemap(argout) doubleArray5 {

    delete $1;
}


%typemap(out) UBoolArray2 {

    $result = PyList_New(arg2);
    for (int32_t i = 0; i < arg2; i++) {
        PyObject *obj = $1[i] ? Py_True : Py_False; Py_INCREF(obj);
        PyList_SET_ITEM($result, i, obj);
    }
}

%typemap(in) UBoolArray4 {

    int len = PyList_Size($input);
    UBool *bools = new UBool[len];
    arg4 = len;
    for (int i = 0; i < len; i++)
        bools[i] = (PyList_GetItem($input, i) == Py_True);
    $1 = (const UBool *) bools;
}
%typemap(argout) UBoolArray4 {

    delete $1;
}

%typemap(in) UBoolArray5 {

    int len = PyList_Size($input);
    UBool *bools = new UBool[len];
    arg5 = len;
    for (int i = 0; i < len; i++)
        bools[i] = (PyList_GetItem($input, i) == Py_True);
    $1 = (const UBool *) bools;
}
%typemap(argout) UBoolArray5 {

    delete $1;
}

%typemap(in) _UConverter * {
    char *encoding = PyString_AsString($input);
    UErrorCode status = U_ZERO_ERROR;
    
    $1 = ucnv_open(encoding, &status);
    if (U_FAILURE(status))
        return ICUException(status).reportError();
}
%typemap(argout) _UConverter * {

    ucnv_close($1);
}
%typecheck(SWIG_TYPECHECK_STRING) _UConverter * {

    $1 = PyString_Check($input);
}

%typemap(in, numinputs=1) (char *, _int32_t) {

    PyString_AsStringAndSize($input, &$1, &$2);
}

%typemap(in, numinputs=1) (char *text, _int32_t len) {

    PyString_AsStringAndSize($input, &$1, &$2);
}


%exception
{
    try {
        $action
    } catch (ICUException e) {
        return e.reportError();
    }
}
