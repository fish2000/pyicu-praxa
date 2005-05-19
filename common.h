/* ====================================================================
 * Copyright (c) 2004-2005 Open Source Applications Foundation.
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

#include <Python.h>
#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/uobject.h>
#include <unicode/rep.h>
#include <unicode/fmtable.h>
#include <unicode/datefmt.h>
#include <unicode/measure.h>
#include <unicode/measunit.h>
#include <unicode/measfmt.h>
#include <unicode/parseerr.h>
#include <unicode/format.h>
#include <unicode/msgfmt.h>

typedef icu::DateFormat _DateFormat;
typedef icu::MeasureFormat _MeasureFormat;
typedef icu::UnicodeString _UnicodeString;
typedef icu::UnicodeString UnicodeString0;
typedef icu::UnicodeString UnicodeString1;
typedef icu::UnicodeString UnicodeString2;
typedef icu::UnicodeString _PyString;
typedef const UChar *ISO3Code;
typedef icu::Formattable Formattable2;
typedef icu::NumberFormat _NumberFormat;

extern PyObject *PyExc_ICUError;

class ICUException {
private:
    PyObject *code;
    PyObject *msg;
public:
    ICUException();
    ICUException(UErrorCode status);
    ICUException(UErrorCode status, char *format, ...);
    ICUException(UParseError &pe, UErrorCode status);
    ~ICUException();
    PyObject *reportError();
};


PyObject *PyUnicode_FromUnicodeString(UnicodeString *string);
UnicodeString &PyUnicode_AsUnicodeString(PyObject *object,
                                         UnicodeString &string);
UnicodeString *PyUnicode_AsUnicodeString(PyObject *object);
UDate PyObject_AsUDate(PyObject *object);

#endif
