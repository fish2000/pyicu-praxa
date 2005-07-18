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

#ifndef _COMMON_H
#define _COMMON_H

#ifdef _MSC_VER

#include <malloc.h>

#define EXPORT __declspec(dllexport)
#define STACK_ARRAY(type, var, len) \
    type *var = (type *) alloca((len) * sizeof(type))

#else

#define EXPORT
#define STACK_ARRAY(type, var, len) \
    type var[len]

#endif

#include <Python.h>
#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/ucnv.h>
#include <unicode/locid.h>
#include <unicode/calendar.h>
#include <unicode/gregocal.h>
#include <unicode/format.h>
#include <unicode/datefmt.h>
#include <unicode/smpdtfmt.h>
#include <unicode/measfmt.h>
#include <unicode/msgfmt.h>
#include <unicode/numfmt.h>
#include <unicode/choicfmt.h>
#include <unicode/decimfmt.h>
#include <unicode/rbnf.h>
#include <unicode/measure.h>
#include <unicode/measunit.h>
#include <unicode/currunit.h>
#include <unicode/curramt.h>
#include <unicode/timezone.h>
#include <unicode/simpletz.h>
#include <unicode/dtfmtsym.h>
#include <unicode/dcfmtsym.h>
#include <unicode/strenum.h>
#include <unicode/chariter.h>
#include <unicode/uchriter.h>
#include <unicode/schriter.h>
#include <unicode/brkiter.h>
#include <unicode/rbbi.h>
#include <unicode/dbbi.h>
#include <unicode/caniter.h>
#include <unicode/coleitr.h>
#include <unicode/coll.h>
#include <unicode/tblcoll.h>


typedef int32_t _int32_t;
typedef const double *doubleArray2;
typedef const double *doubleArray3;
typedef const double *doubleArray4;
typedef const double *doubleArray5;
typedef const UBool *UBoolArray2;
typedef const UBool *UBoolArray4;
typedef const UBool *UBoolArray5;
typedef const icu::Locale *LocaleDict1;
typedef const icu::UnicodeString *UnicodeStringArray2;
typedef const icu::UnicodeString *UnicodeStringArray3;
typedef const icu::UnicodeString *UnicodeStringArray4;
typedef const icu::UnicodeString *UnicodeStringArray5;
typedef const icu::UnicodeString *LeakyUnicodeStringArray3;
typedef const icu::Format **FormatPointerArray2;
typedef const icu::Format **FormatPointerArray3;
typedef icu::DateFormat _DateFormat;
typedef icu::MeasureFormat _MeasureFormat;
typedef icu::Calendar _Calendar;
typedef icu::TimeZone _TimeZone;
typedef icu::TimeZone TimeZone_;
typedef icu::StringEnumeration _StringEnumeration;
typedef icu::Collator _Collator;
typedef const icu::TimeZone const_TimeZone;
typedef icu::UnicodeString _UnicodeString;
typedef icu::UnicodeString UnicodeString0;
typedef icu::UnicodeString UnicodeString1;
typedef icu::UnicodeString UnicodeString2;
typedef icu::UnicodeString UnicodeString3;
typedef icu::UnicodeString _PyString;
typedef icu::CollationKey CollationKey2;
typedef const UChar *ISO3Code;
typedef icu::Formattable Formattable2;
typedef const icu::Formattable *FormattableArray3;
typedef const icu::Formattable *FormattableArray4;
typedef icu::NumberFormat _NumberFormat;
typedef UConverter _UConverter;

EXPORT void setICUErrorClass(PyObject *);

class ICUException {
private:
    PyObject *code;
    PyObject *msg;
public:
    EXPORT ICUException();
    EXPORT ICUException(UErrorCode status);
    EXPORT ICUException(UErrorCode status, char *format, ...);
    EXPORT ICUException(UParseError &pe, UErrorCode status);
    EXPORT ~ICUException();
    EXPORT PyObject *reportError();
};

EXPORT PyObject *PyUnicode_FromUnicodeString(UnicodeString *string);

EXPORT UnicodeString &PyString_AsUnicodeString(PyObject *object,
                                               char *encoding, char *mode,
                                               UnicodeString &string);
EXPORT UnicodeString &PyObject_AsUnicodeString(PyObject *object,
                                               char *encoding, char *mode,
                                               UnicodeString &string);
EXPORT UnicodeString &PyObject_AsUnicodeString(PyObject *object,
                                               UnicodeString &string);
EXPORT UnicodeString *PyObject_AsUnicodeString(PyObject *object);
EXPORT UDate PyObject_AsUDate(PyObject *object);

#endif
