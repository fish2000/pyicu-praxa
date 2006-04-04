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

#ifndef _common_h
#define _common_h

#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#include <Python.h>
#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/ucnv.h>
#include <unicode/locid.h>
#include <unicode/resbund.h>
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

extern PyObject *PyExc_ICUError;
extern PyObject *PyExc_InvalidArgsError;

enum {
    UObject_ID,
    Replaceable_ID,
    MeasureUnit_ID,
    Measure_ID,
    StringEnumeration_ID,
    ForwardCharacterIterator_ID,
    CharacterIterator_ID,
    BreakIterator_ID,
    Format_ID,
    MeasureFormat_ID,
    DateFormat_ID,
    Calendar_ID,
    Collator_ID,
};

void _init_common(PyObject *m);

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

int abstract_init(PyObject *self, PyObject *args, PyObject *kwds);

#define parseArgs(args, types, rest...) \
    _parseArgs(((PyTupleObject *)(args))->ob_item, \
               ((PyTupleObject *)(args))->ob_size, types, ##rest)

#define parseArg(arg, types, rest...) \
    _parseArgs(&(arg), 1, types, ##rest)

int _parseArgs(PyObject **args, int count, char *types, ...);
int isUnicodeString(PyObject *arg);
int isInstance(PyObject *arg, UClassID id, PyTypeObject *type);
void registerType(PyTypeObject *type, UClassID id);

icu::Formattable *toFormattableArray(PyObject *arg, int *len,
                                     UClassID id, PyTypeObject *type);

icu::UObject **pl2cpa(PyObject *arg, int *len, UClassID id, PyTypeObject *type);
PyObject *cpa2pl(icu::UObject **array, int len,
                 PyObject *(*wrap)(UObject *, int));

PyObject *PyErr_SetArgsError(PyObject *self, char *name, PyObject *args);
PyObject *PyErr_SetArgsError(PyTypeObject *type, char *name, PyObject *args);

#endif /* _common_h */
