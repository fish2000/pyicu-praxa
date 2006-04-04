/* ====================================================================
 * Copyright (c) 2004-2006 Open Source Applications Foundation.
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
#include "structmember.h"

#include "bases.h"
#include "locale.h"
#include "format.h"
#include "calendar.h"
#include "numberformat.h"
#include "dateformat.h"
#include "macros.h"


/* DateFormatSymbols */

class t_dateformatsymbols : public _wrapper {
public:
    icu::DateFormatSymbols *object;
};

static int t_dateformatsymbols_init(t_dateformatsymbols *self,
                                    PyObject *args, PyObject *kwds);
static PyObject *t_dateformatsymbols_getEras(t_dateformatsymbols *self);
static PyObject *t_dateformatsymbols_setEras(t_dateformatsymbols *self, PyObject *arg);
static PyObject *t_dateformatsymbols_getMonths(t_dateformatsymbols *self,
                                               PyObject *args);
static PyObject *t_dateformatsymbols_setMonths(t_dateformatsymbols *self,
                                               PyObject *arg);
static PyObject *t_dateformatsymbols_getShortMonths(t_dateformatsymbols *self);
static PyObject *t_dateformatsymbols_setShortMonths(t_dateformatsymbols *self,
                                                    PyObject *arg);
static PyObject *t_dateformatsymbols_getWeekdays(t_dateformatsymbols *self,
                                                 PyObject *args);
static PyObject *t_dateformatsymbols_setWeekdays(t_dateformatsymbols *self,
                                                 PyObject *arg);
static PyObject *t_dateformatsymbols_getShortWeekdays(t_dateformatsymbols *self);
static PyObject *t_dateformatsymbols_setShortWeekdays(t_dateformatsymbols *self,
                                                      PyObject *arg);
static PyObject *t_dateformatsymbols_getAmPmStrings(t_dateformatsymbols *self);
static PyObject *t_dateformatsymbols_setAmPmStrings(t_dateformatsymbols *self,
                                                    PyObject *arg);
static PyObject *t_dateformatsymbols_getLocalPatternChars(t_dateformatsymbols *self, PyObject *args);
static PyObject *t_dateformatsymbols_setLocalPatternChars(t_dateformatsymbols *self, PyObject *arg);
static PyObject *t_dateformatsymbols_getLocale(t_dateformatsymbols *self,
                                               PyObject *args);

static PyMethodDef t_dateformatsymbols_methods[] = {
    DECLARE_METHOD(t_dateformatsymbols, getEras, METH_NOARGS),
    DECLARE_METHOD(t_dateformatsymbols, setEras, METH_O),
    DECLARE_METHOD(t_dateformatsymbols, getMonths, METH_VARARGS),
    DECLARE_METHOD(t_dateformatsymbols, setMonths, METH_O),
    DECLARE_METHOD(t_dateformatsymbols, getShortMonths, METH_NOARGS),
    DECLARE_METHOD(t_dateformatsymbols, setShortMonths, METH_O),
    DECLARE_METHOD(t_dateformatsymbols, getWeekdays, METH_VARARGS),
    DECLARE_METHOD(t_dateformatsymbols, setWeekdays, METH_O),
    DECLARE_METHOD(t_dateformatsymbols, getShortWeekdays, METH_NOARGS),
    DECLARE_METHOD(t_dateformatsymbols, setShortWeekdays, METH_O),
    DECLARE_METHOD(t_dateformatsymbols, getAmPmStrings, METH_NOARGS),
    DECLARE_METHOD(t_dateformatsymbols, setAmPmStrings, METH_O),
    DECLARE_METHOD(t_dateformatsymbols, getLocalPatternChars, METH_VARARGS),
    DECLARE_METHOD(t_dateformatsymbols, setLocalPatternChars, METH_O),
    DECLARE_METHOD(t_dateformatsymbols, getLocale, METH_VARARGS),
    { NULL, NULL, 0, NULL }
};

DECLARE_TYPE(DateFormatSymbols, t_dateformatsymbols, UObject,
             icu::DateFormatSymbols, t_dateformatsymbols_init);

/* DateFormat */

class t_dateformat : public _wrapper {
public:
    icu::DateFormat *object;
};

static PyObject *t_dateformat_isLenient(t_dateformat *self);
static PyObject *t_dateformat_setLenient(t_dateformat *self, PyObject *arg);
static PyObject *t_dateformat_format(t_dateformat *self, PyObject *args);
static PyObject *t_dateformat_parse(t_dateformat *self, PyObject *args);
static PyObject *t_dateformat_getCalendar(t_dateformat *self);
static PyObject *t_dateformat_setCalendar(t_dateformat *self, PyObject *arg);
static PyObject *t_dateformat_getNumberFormat(t_dateformat *self);
static PyObject *t_dateformat_setNumberFormat(t_dateformat *self, PyObject *arg);
static PyObject *t_dateformat_getTimeZone(t_dateformat *self);
static PyObject *t_dateformat_setTimeZone(t_dateformat *self, PyObject *arg);
static PyObject *t_dateformat_createInstance(PyTypeObject *type);
static PyObject *t_dateformat_createTimeInstance(PyTypeObject *type,
                                                 PyObject *args);
static PyObject *t_dateformat_createDateInstance(PyTypeObject *type,
                                                 PyObject *args);
static PyObject *t_dateformat_createDateTimeInstance(PyTypeObject *type,
                                                     PyObject *args);
static PyObject *t_dateformat_getAvailableLocales(PyTypeObject *type);

static PyMethodDef t_dateformat_methods[] = {
    DECLARE_METHOD(t_dateformat, isLenient, METH_NOARGS),
    DECLARE_METHOD(t_dateformat, setLenient, METH_O),
    DECLARE_METHOD(t_dateformat, format, METH_VARARGS),
    DECLARE_METHOD(t_dateformat, parse, METH_VARARGS),
    DECLARE_METHOD(t_dateformat, getCalendar, METH_NOARGS),
    DECLARE_METHOD(t_dateformat, setCalendar, METH_O),
    DECLARE_METHOD(t_dateformat, getNumberFormat, METH_NOARGS),
    DECLARE_METHOD(t_dateformat, setNumberFormat, METH_O),
    DECLARE_METHOD(t_dateformat, getTimeZone, METH_NOARGS),
    DECLARE_METHOD(t_dateformat, setTimeZone, METH_O),
    DECLARE_METHOD(t_dateformat, createInstance, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_dateformat, createTimeInstance, METH_VARARGS | METH_CLASS),
    DECLARE_METHOD(t_dateformat, createDateInstance, METH_VARARGS | METH_CLASS),
    DECLARE_METHOD(t_dateformat, createDateTimeInstance, METH_VARARGS | METH_CLASS),
    DECLARE_METHOD(t_dateformat, getAvailableLocales, METH_NOARGS | METH_CLASS),
    { NULL, NULL, 0, NULL }
};

DECLARE_TYPE(DateFormat, t_dateformat, Format, icu::DateFormat, abstract_init);

/* SimpleDateFormat */

class t_simpledateformat : public _wrapper {
public:
    icu::SimpleDateFormat *object;
};

static int t_simpledateformat_init(t_simpledateformat *self,
                                   PyObject *args, PyObject *kwds);
static PyObject *t_simpledateformat_toPattern(t_simpledateformat *self,
                                              PyObject *args);
static PyObject *t_simpledateformat_toLocalizedPattern(t_simpledateformat *self,
                                                       PyObject *args);
static PyObject *t_simpledateformat_applyPattern(t_simpledateformat *self,
                                                 PyObject *arg);
static PyObject *t_simpledateformat_applyLocalizedPattern(t_simpledateformat *self, PyObject *arg);
static PyObject *t_simpledateformat_get2DigitYearStart(t_simpledateformat *self);
static PyObject *t_simpledateformat_set2DigitYearStart(t_simpledateformat *self,
                                                       PyObject *arg);
static PyObject *t_simpledateformat_getDateFormatSymbols(t_simpledateformat *self);
static PyObject *t_simpledateformat_setDateFormatSymbols(t_simpledateformat *self,
                                                         PyObject *arg);

static PyMethodDef t_simpledateformat_methods[] = {
    DECLARE_METHOD(t_simpledateformat, toPattern, METH_VARARGS),
    DECLARE_METHOD(t_simpledateformat, toLocalizedPattern, METH_VARARGS),
    DECLARE_METHOD(t_simpledateformat, applyPattern, METH_O),
    DECLARE_METHOD(t_simpledateformat, applyLocalizedPattern, METH_O),
    DECLARE_METHOD(t_simpledateformat, get2DigitYearStart, METH_NOARGS),
    DECLARE_METHOD(t_simpledateformat, set2DigitYearStart, METH_O),
    DECLARE_METHOD(t_simpledateformat, getDateFormatSymbols, METH_NOARGS),
    DECLARE_METHOD(t_simpledateformat, setDateFormatSymbols, METH_O),
    { NULL, NULL, 0, NULL }
};

DECLARE_TYPE(SimpleDateFormat, t_simpledateformat, DateFormat,
             icu::SimpleDateFormat, t_simpledateformat_init);

PyObject *wrap_DateFormat(icu::DateFormat *format)
{
    if (format->getDynamicClassID() == icu::SimpleDateFormat::getStaticClassID())
        return wrap_SimpleDateFormat((icu::SimpleDateFormat *) format, T_OWNED);

    return wrap_DateFormat(format, T_OWNED);
}


/* DateFormatSymbols */

static int t_dateformatsymbols_init(t_dateformatsymbols *self,
                                    PyObject *args, PyObject *kwds)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;
    icu::Locale *locale;
    icu::DateFormatSymbols *dfs;
    char *type;

    switch (PyTuple_Size(args)) {
      case 0:
        INT_STATUS_CALL(dfs = new icu::DateFormatSymbols(status));
        self->object = dfs;
        self->flags = T_OWNED;
        break;
      case 1:
        if (!parseArgs(args, "P", TYPE_CLASSID(Locale), &locale))
        {
            INT_STATUS_CALL(dfs = new icu::DateFormatSymbols(*locale, status));
            self->object = dfs;
            self->flags = T_OWNED;
            break;
        }
        if (!parseArgs(args, "c", &type))
        {
            INT_STATUS_CALL(dfs = new icu::DateFormatSymbols(type, status));
            self->object = dfs;
            self->flags = T_OWNED;
            break;
        }
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
      case 2:
        if (!parseArgs(args, "Pc", TYPE_CLASSID(Locale),
                       &locale, &type))
        {
            INT_STATUS_CALL(dfs = new icu::DateFormatSymbols(*locale, type,
                                                             status));
            self->object = dfs;
            self->flags = T_OWNED;
            break;
        }
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
      default:
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
    }
        
    if (self->object)
        return 0;

    return -1;
}

static PyObject *fromUnicodeStringArray(const icu::UnicodeString *strings,
                                        int len, int dispose)
{
    PyObject *list = PyList_New(len);
    
    for (int i = 0; i < len; i++) {
        icu::UnicodeString *u = (icu::UnicodeString *) (strings + i);
        PyList_SET_ITEM(list, i, PyUnicode_FromUnicodeString(u));
    }

    if (dispose)
        delete strings;

    return list;
}

static PyObject *t_dateformatsymbols_getEras(t_dateformatsymbols *self)
{
    int len;
    const UnicodeString *eras = self->object->getEras(len);

    return fromUnicodeStringArray(eras, len, 0);
}

static PyObject *t_dateformatsymbols_setEras(t_dateformatsymbols *self,
                                             PyObject *arg)
{
    icu::UnicodeString *eras;
    int len;

    if (!parseArg(arg, "T", &eras, &len))
    {
        self->object->setEras(eras, len);
        delete[] eras; /* dtfmtsym.cpp code duplicates it */
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setEras", arg);
}

static PyObject *t_dateformatsymbols_getMonths(t_dateformatsymbols *self,
                                               PyObject *args)
{
    int len;
    const icu::UnicodeString *months;
    icu::DateFormatSymbols::DtContextType context;
    icu::DateFormatSymbols::DtWidthType width;

    switch (PyTuple_Size(args)) {
      case 0:
        months = self->object->getMonths(len);
        return fromUnicodeStringArray(months, len, 0);
      case 2:
        if (!parseArgs(args, "ii", &context, &width))
        {
            months = self->object->getMonths(len, context, width);
            return fromUnicodeStringArray(months, len, 0);
        }
        break;
    }
            
    return PyErr_SetArgsError((PyObject *) self, "getMonths", args);
}

static PyObject *t_dateformatsymbols_setMonths(t_dateformatsymbols *self,
                                               PyObject *arg)
{
    icu::UnicodeString *months;
    int len;

    if (!parseArg(arg, "T", &months, &len))
    {
        self->object->setMonths(months, len);
        delete[] months; /* dtfmtsym.cpp code duplicates it */
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setMonths", arg);
}

static PyObject *t_dateformatsymbols_getShortMonths(t_dateformatsymbols *self)
{
    int len;
    const UnicodeString *months = self->object->getShortMonths(len);

    return fromUnicodeStringArray(months, len, 0);
}

static PyObject *t_dateformatsymbols_setShortMonths(t_dateformatsymbols *self,
                                                    PyObject *arg)
{
    icu::UnicodeString *months;
    int len;

    if (!parseArg(arg, "T", &months, &len))
    {
        self->object->setShortMonths(months, len);
        delete[] months; /* dtfmtsym.cpp code duplicates it */
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setShortMonths", arg);
}

static PyObject *t_dateformatsymbols_getWeekdays(t_dateformatsymbols *self,
                                                 PyObject *args)
{
    int len;
    const icu::UnicodeString *weekdays;
    icu::DateFormatSymbols::DtContextType context;
    icu::DateFormatSymbols::DtWidthType width;

    switch (PyTuple_Size(args)) {
      case 0:
        weekdays = self->object->getWeekdays(len);
        return fromUnicodeStringArray(weekdays, len, 0);
      case 2:
        if (!parseArgs(args, "ii", &context, &width))
        {
            weekdays = self->object->getWeekdays(len, context, width);
            return fromUnicodeStringArray(weekdays, len, 0);
        }
        break;
    }
            
    return PyErr_SetArgsError((PyObject *) self, "getWeekdays", args);
}

static PyObject *t_dateformatsymbols_setWeekdays(t_dateformatsymbols *self,
                                                 PyObject *arg)
{
    icu::UnicodeString *weekdays;
    int len;

    if (!parseArg(arg, "T", &weekdays, &len))
    {
        self->object->setWeekdays(weekdays, len);
        delete[] weekdays; /* dtfmtsym.cpp code duplicates it */
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setWeekdays", arg);
}

static PyObject *t_dateformatsymbols_getShortWeekdays(t_dateformatsymbols *self)
{
    int len;
    const UnicodeString *months = self->object->getShortWeekdays(len);

    return fromUnicodeStringArray(months, len, 0);
}

static PyObject *t_dateformatsymbols_setShortWeekdays(t_dateformatsymbols *self,
                                                      PyObject *arg)
{
    icu::UnicodeString *weekdays;
    int len;

    if (!parseArg(arg, "T", &weekdays, &len))
    {
        self->object->setShortWeekdays(weekdays, len);
        delete[] weekdays; /* dtfmtsym.cpp code duplicates it */
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setShortWeekdays", arg);
}

static PyObject *t_dateformatsymbols_getAmPmStrings(t_dateformatsymbols *self)
{
    int len;
    const UnicodeString *strings = self->object->getAmPmStrings(len);

    return fromUnicodeStringArray(strings, len, 0);
}

static PyObject *t_dateformatsymbols_setAmPmStrings(t_dateformatsymbols *self,
                                                    PyObject *arg)
{
    icu::UnicodeString *strings;
    int len;

    if (!parseArg(arg, "T", &strings, &len))
    {
        self->object->setAmPmStrings(strings, len);
        delete[] strings; /* dtfmtsym.cpp code duplicates it */
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setAmPmStrings", arg);
}

static PyObject *t_dateformatsymbols_richcmp(t_dateformatsymbols *self,
                                             PyObject *arg, int op)
{
    int b = 0;
    icu::DateFormatSymbols *dfs;

    if (!parseArg(arg, "P", TYPE_CLASSID(DateFormatSymbols), &dfs))
    {
        switch (op) {
          case Py_EQ:
          case Py_NE:
            b = *self->object == *dfs;
            if (op == Py_EQ)
                Py_RETURN_BOOL(b);
            Py_RETURN_BOOL(!b);
          case Py_LT:
          case Py_LE:
          case Py_GT:
          case Py_GE:
            PyErr_SetNone(PyExc_NotImplementedError);
            return NULL;
        }
    }

    return PyErr_SetArgsError((PyObject *) self, "__richcmp__", arg);
}
        
static PyObject *t_dateformatsymbols_getLocalPatternChars(t_dateformatsymbols *self, PyObject *args)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    switch (PyTuple_Size(args)) {
      case 0:
        self->object->getLocalPatternChars(_u);
        return PyUnicode_FromUnicodeString(&_u);
      case 1:
        if (!parseArgs(args, "U", &u))
        {
            self->object->getLocalPatternChars(*u);
            Py_RETURN_ARG(args, 0);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "getLocalPatternChars", args);
}

static PyObject *t_dateformatsymbols_setLocalPatternChars(t_dateformatsymbols *self, PyObject *arg)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    if (!parseArg(arg, "S", &u, &_u))
    {
        self->object->setLocalPatternChars(*u);
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setLocalPatternChars", arg);
}

static PyObject *t_dateformatsymbols_getLocale(t_dateformatsymbols *self,
                                               PyObject *args)
{
    ULocDataLocaleType type;
    icu::Locale locale;

    switch (PyTuple_Size(args)) {
      case 0:
        STATUS_CALL(locale = self->object->getLocale(ULOC_VALID_LOCALE,
                                                     status));
        return wrap_Locale(locale);
      case 1:
        if (!parseArgs(args, "i", &type))
        {
            STATUS_CALL(locale = self->object->getLocale(type, status));
            return wrap_Locale(locale);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "getLocale", args);
}


/* DateFormat */

static PyObject *t_dateformat_isLenient(t_dateformat *self)
{
    int b = self->object->isLenient();
    Py_RETURN_BOOL(b);
}

static PyObject *t_dateformat_setLenient(t_dateformat *self, PyObject *arg)
{
    int b;

    if (!parseArg(arg, "b", &b))
    {
        self->object->setLenient(b);
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setLenient", arg);
}

static PyObject *t_dateformat_format(t_dateformat *self, PyObject *args)
{
    UDate date;
    icu::Calendar *calendar;
    icu::UnicodeString *u;
    icu::UnicodeString _u;
    icu::FieldPosition *fp;

    switch (PyTuple_Size(args)) {
      case 1:
        if (!parseArgs(args, "D", &date))
        {
            self->object->format(date, _u);
            return PyUnicode_FromUnicodeString(&_u);
        }
        break;
      case 2:
        if (!parseArgs(args, "DP", TYPE_CLASSID(FieldPosition),
                       &date, &fp))
        {
            self->object->format(date, _u, *fp);
            return PyUnicode_FromUnicodeString(&_u);
        }
        if (!parseArgs(args, "PP",
                       TYPE_ID(Calendar), TYPE_CLASSID(FieldPosition),
                       &calendar, &fp))
        {
            self->object->format(*calendar, _u, *fp);
            return PyUnicode_FromUnicodeString(&_u);
        }
        if (!parseArgs(args, "DU", &date, &u))
        {
            self->object->format(date, *u);
            Py_RETURN_ARG(args, 1);
        }
        break;
      case 3:
        if (!parseArgs(args, "DUP", TYPE_CLASSID(FieldPosition),
                       &date, &u, &fp))
        {
            self->object->format(date, *u, *fp);
            Py_RETURN_ARG(args, 1);
        }
        if (!parseArgs(args, "PUP",
                       TYPE_ID(Calendar), TYPE_CLASSID(FieldPosition),
                       &calendar, &u, &fp))
        {
            self->object->format(*calendar, *u, *fp);
            Py_RETURN_ARG(args, 1);
        }
        break;
    }

    return t_format_format((t_format *) self, args);
}

static PyObject *t_dateformat_parse(t_dateformat *self, PyObject *args)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;
    icu::Calendar *calendar;
    icu::ParsePosition *pp;
    UDate date;

    switch (PyTuple_Size(args)) {
      case 1:
        if (!parseArgs(args, "S", &u, &_u))
        {
            STATUS_CALL(date = self->object->parse(*u, status));
            return PyFloat_FromDouble(date / 1000.0);
        }
        break;
      case 2:
        if (!parseArgs(args, "SP", TYPE_CLASSID(ParsePosition),
                       &u, &_u, &pp))
        {
            pp->setErrorIndex(-1);
            STATUS_CALL(date = self->object->parse(*u, *pp));
            if (pp->getErrorIndex() == -1)
                Py_RETURN_NONE;
            return PyFloat_FromDouble(date / 1000.0);
        }
        break;
      case 3:
        if (!parseArgs(args, "SPP",
                       TYPE_ID(Calendar), TYPE_CLASSID(ParsePosition),
                       &u, &_u, &calendar, &pp))
        {
            pp->setErrorIndex(-1);
            STATUS_CALL(self->object->parse(*u, *calendar, *pp));
            Py_RETURN_NONE;
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "parse", args);
}

static PyObject *t_dateformat_getCalendar(t_dateformat *self)
{
    return wrap_Calendar(self->object->getCalendar()->clone(), T_OWNED);
}

static PyObject *t_dateformat_setCalendar(t_dateformat *self, PyObject *arg)
{
    icu::Calendar *calendar;

    if (!parseArg(arg, "P", TYPE_ID(Calendar), &calendar))
    {
        self->object->setCalendar(*calendar);
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setCalendar", arg);
}

static PyObject *t_dateformat_getNumberFormat(t_dateformat *self)
{
    return wrap_NumberFormat((icu::NumberFormat *) self->object->getNumberFormat()->clone(), T_OWNED);
}

static PyObject *t_dateformat_setNumberFormat(t_dateformat *self, PyObject *arg)
{
    icu::NumberFormat *format;

    if (!parseArg(arg, "P", TYPE_CLASSID(NumberFormat), &format))
    {
        self->object->setNumberFormat(*format);
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setNumberFormat", arg);
}

static PyObject *t_dateformat_getTimeZone(t_dateformat *self)
{
    return wrap_TimeZone(self->object->getTimeZone());
}

static PyObject *t_dateformat_setTimeZone(t_dateformat *self, PyObject *arg)
{
    icu::TimeZone *tz;

    if (!parseArg(arg, "P", TYPE_CLASSID(TimeZone), &tz))
    {
        self->object->setTimeZone(*tz);
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setTimeZone", arg);
}

static PyObject *t_dateformat_createInstance(PyTypeObject *type)
{
    return wrap_DateFormat(icu::DateFormat::createInstance());
}

static PyObject *t_dateformat_createTimeInstance(PyTypeObject *type,
                                                 PyObject *args)
{
    icu::DateFormat::EStyle style;
    icu::Locale *locale;

    switch (PyTuple_Size(args)) {
      case 1:
        if (!parseArgs(args, "i", &style))
            return wrap_DateFormat(icu::DateFormat::createTimeInstance(style));
        break;
      case 2:
        if (!parseArgs(args, "iP", TYPE_CLASSID(Locale),
                       &style, &locale))
            return wrap_DateFormat(icu::DateFormat::createTimeInstance(style, *locale));
        break;
    }

    return PyErr_SetArgsError(type, "createTimeInstance", args);
}

static PyObject *t_dateformat_createDateInstance(PyTypeObject *type,
                                                 PyObject *args)
{
    icu::DateFormat::EStyle style;
    icu::Locale *locale;

    switch (PyTuple_Size(args)) {
      case 1:
        if (!parseArgs(args, "i", &style))
            return wrap_DateFormat(icu::DateFormat::createDateInstance(style));
        break;
      case 2:
        if (!parseArgs(args, "iP", TYPE_CLASSID(Locale),
                       &style, &locale))
            return wrap_DateFormat(icu::DateFormat::createDateInstance(style, *locale));
        break;
    }

    return PyErr_SetArgsError(type, "createDateInstance", args);
}

static PyObject *t_dateformat_createDateTimeInstance(PyTypeObject *type,
                                                     PyObject *args)
{
    icu::DateFormat::EStyle dateStyle, timeStyle;
    icu::Locale *locale;

    switch (PyTuple_Size(args)) {
      case 1:
        if (!parseArgs(args, "i", &dateStyle))
            return wrap_DateFormat(icu::DateFormat::createDateTimeInstance(dateStyle));
        break;
      case 2:
        if (!parseArgs(args, "ii", &dateStyle, &timeStyle))
            return wrap_DateFormat(icu::DateFormat::createDateTimeInstance(dateStyle, timeStyle));
        break;
      case 3:
        if (!parseArgs(args, "iiP", TYPE_CLASSID(Locale),
                       &dateStyle, &timeStyle, &locale))
            return wrap_DateFormat(icu::DateFormat::createDateTimeInstance(dateStyle, timeStyle, *locale));
        break;
    }

    return PyErr_SetArgsError(type, "createDateTimeInstance", args);
}

static PyObject *t_dateformat_getAvailableLocales(PyTypeObject *type)
{
    int count;
    const icu::Locale *locales = icu::DateFormat::getAvailableLocales(count);
    PyObject *dict = PyDict_New();

    for (int32_t i = 0; i < count; i++) {
        Locale *locale = (Locale *) locales + i;
        PyObject *obj = wrap_Locale(locale, 0);
        PyDict_SetItemString(dict, locale->getName(), obj);
	Py_DECREF(obj);
    }

    return dict;
}


/* SimpleDateFormat */

static int t_simpledateformat_init(t_simpledateformat *self,
                                   PyObject *args, PyObject *kwds)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;
    icu::Locale *locale;
    icu::DateFormatSymbols *dfs;
    icu::SimpleDateFormat *format;

    switch (PyTuple_Size(args)) {
      case 0:
        INT_STATUS_CALL(format = new icu::SimpleDateFormat(status));
        self->object = format;
        self->flags = T_OWNED;
        break;
      case 1:
        if (!parseArgs(args, "S", &u, &_u))
        {
            INT_STATUS_CALL(format = new icu::SimpleDateFormat(*u, status));
            self->object = format;
            self->flags = T_OWNED;
            break;
        }
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
      case 2:
        if (!parseArgs(args, "SP", TYPE_CLASSID(Locale),
                       &u, &_u, &locale))
        {
            INT_STATUS_CALL(format = new icu::SimpleDateFormat(*u, *locale,
                                                               status));
            self->object = format;
            self->flags = T_OWNED;
            break;
        }
        if (!parseArgs(args, "SP", TYPE_CLASSID(DateFormatSymbols),
                       &u, &_u, &dfs))
        {
            INT_STATUS_CALL(format = new icu::SimpleDateFormat(*u, *dfs,
                                                               status));
            self->object = format;
            self->flags = T_OWNED;
            break;
        }
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
      default:
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
    }
        
    if (self->object)
        return 0;

    return -1;
}

static PyObject *t_simpledateformat_toPattern(t_simpledateformat *self,
                                              PyObject *args)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    switch (PyTuple_Size(args)) {
      case 0:
        self->object->toPattern(_u);
        return PyUnicode_FromUnicodeString(&_u);
      case 1:
        if (!parseArgs(args, "U", &u))
        {
            self->object->toPattern(*u);
            Py_RETURN_ARG(args, 0);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "toPattern", args);
}

static PyObject *t_simpledateformat_toLocalizedPattern(t_simpledateformat *self,
                                                       PyObject *args)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    switch (PyTuple_Size(args)) {
      case 0:
        STATUS_CALL(self->object->toLocalizedPattern(_u, status));
        return PyUnicode_FromUnicodeString(&_u);
      case 1:
        if (!parseArgs(args, "U", &u))
        {
            STATUS_CALL(self->object->toLocalizedPattern(*u, status));
            Py_RETURN_ARG(args, 0);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "toLocalizedPattern", args);
}

static PyObject *t_simpledateformat_applyPattern(t_simpledateformat *self,
                                                 PyObject *arg)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    if (!parseArg(arg, "S", &u, &_u))
    {
        self->object->applyPattern(*u);
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "applyPattern", arg);
}

static PyObject *t_simpledateformat_applyLocalizedPattern(t_simpledateformat *self, PyObject *arg)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    if (!parseArg(arg, "S", &u, &_u))
    {
        STATUS_CALL(self->object->applyLocalizedPattern(*u, status));
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "applyLocalizedPattern", arg);
}

static PyObject *t_simpledateformat_get2DigitYearStart(t_simpledateformat *self)
{
    UDate date;

    STATUS_CALL(date = self->object->get2DigitYearStart(status));
    return PyFloat_FromDouble(date / 1000.0);
}

static PyObject *t_simpledateformat_set2DigitYearStart(t_simpledateformat *self,
                                                       PyObject *arg)
{
    UDate date;

    if (!parseArg(arg, "D", &date))
    {
        STATUS_CALL(self->object->set2DigitYearStart(date, status));
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "set2DigitYearStart", arg);
}

static PyObject *t_simpledateformat_getDateFormatSymbols(t_simpledateformat *self)
{
    return wrap_DateFormatSymbols(new DateFormatSymbols(*self->object->getDateFormatSymbols()), T_OWNED);
}

static PyObject *t_simpledateformat_setDateFormatSymbols(t_simpledateformat *self, PyObject *arg)
{
    icu::DateFormatSymbols *dfs;

    if (!parseArg(arg, "P", TYPE_CLASSID(DateFormatSymbols), &dfs))
    {
        self->object->setDateFormatSymbols(*dfs);
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setDateFormatSymbols", arg);
}

static PyObject *t_simpledateformat_str(t_simpledateformat *self)
{
    UnicodeString u; 

    self->object->toPattern(u);
    return PyUnicode_FromUnicodeString(&u);
}


void _init_dateformat(PyObject *m)
{
    DateFormatSymbolsType.tp_richcompare =
        (richcmpfunc) t_dateformatsymbols_richcmp;
    SimpleDateFormatType.tp_str = (reprfunc) t_simpledateformat_str;

    REGISTER_TYPE(DateFormatSymbols, m);
    INSTALL_TYPE(DateFormat, m);
    REGISTER_TYPE(SimpleDateFormat, m);

    INSTALL_STATIC_INT(DateFormatSymbols, FORMAT);
    INSTALL_STATIC_INT(DateFormatSymbols, STANDALONE);

    INSTALL_STATIC_INT(DateFormatSymbols, WIDE);
    INSTALL_STATIC_INT(DateFormatSymbols, ABBREVIATED);
    INSTALL_STATIC_INT(DateFormatSymbols, NARROW);

    INSTALL_STATIC_INT(DateFormat, kNone);
    INSTALL_STATIC_INT(DateFormat, kFull);
    INSTALL_STATIC_INT(DateFormat, kLong);
    INSTALL_STATIC_INT(DateFormat, kMedium);
    INSTALL_STATIC_INT(DateFormat, kShort);
    INSTALL_STATIC_INT(DateFormat, kDateOffset);
    INSTALL_STATIC_INT(DateFormat, kDateTime);
    INSTALL_STATIC_INT(DateFormat, kDefault);
    INSTALL_STATIC_INT(DateFormat, FULL);
    INSTALL_STATIC_INT(DateFormat, LONG);
    INSTALL_STATIC_INT(DateFormat, MEDIUM);
    INSTALL_STATIC_INT(DateFormat, SHORT);
    INSTALL_STATIC_INT(DateFormat, DEFAULT);
    INSTALL_STATIC_INT(DateFormat, DATE_OFFSET);
    INSTALL_STATIC_INT(DateFormat, NONE);
    INSTALL_STATIC_INT(DateFormat, DATE_TIME);

    INSTALL_STATIC_INT(DateFormat, kEraField);
    INSTALL_STATIC_INT(DateFormat, kYearField);
    INSTALL_STATIC_INT(DateFormat, kMonthField);
    INSTALL_STATIC_INT(DateFormat, kDateField);
    INSTALL_STATIC_INT(DateFormat, kHourOfDay1Field);
    INSTALL_STATIC_INT(DateFormat, kHourOfDay0Field);
    INSTALL_STATIC_INT(DateFormat, kMinuteField);
    INSTALL_STATIC_INT(DateFormat, kSecondField);
    INSTALL_STATIC_INT(DateFormat, kMillisecondField);
    INSTALL_STATIC_INT(DateFormat, kDayOfWeekField);
    INSTALL_STATIC_INT(DateFormat, kDayOfYearField);
    INSTALL_STATIC_INT(DateFormat, kDayOfWeekInMonthField);
    INSTALL_STATIC_INT(DateFormat, kWeekOfYearField);
    INSTALL_STATIC_INT(DateFormat, kWeekOfMonthField);
    INSTALL_STATIC_INT(DateFormat, kAmPmField);
    INSTALL_STATIC_INT(DateFormat, kHour1Field);
    INSTALL_STATIC_INT(DateFormat, kHour0Field);
    INSTALL_STATIC_INT(DateFormat, kTimezoneField);
    INSTALL_STATIC_INT(DateFormat, kYearWOYField);
    INSTALL_STATIC_INT(DateFormat, kDOWLocalField);
    INSTALL_STATIC_INT(DateFormat, kExtendedYearField);
    INSTALL_STATIC_INT(DateFormat, kJulianDayField);
    INSTALL_STATIC_INT(DateFormat, kMillisecondsInDayField);
    INSTALL_STATIC_INT(DateFormat, ERA_FIELD);
    INSTALL_STATIC_INT(DateFormat, YEAR_FIELD);
    INSTALL_STATIC_INT(DateFormat, MONTH_FIELD);
    INSTALL_STATIC_INT(DateFormat, DATE_FIELD);
    INSTALL_STATIC_INT(DateFormat, HOUR_OF_DAY1_FIELD);
    INSTALL_STATIC_INT(DateFormat, HOUR_OF_DAY0_FIELD);
    INSTALL_STATIC_INT(DateFormat, MINUTE_FIELD);
    INSTALL_STATIC_INT(DateFormat, SECOND_FIELD);
    INSTALL_STATIC_INT(DateFormat, MILLISECOND_FIELD);
    INSTALL_STATIC_INT(DateFormat, DAY_OF_WEEK_FIELD);
    INSTALL_STATIC_INT(DateFormat, DAY_OF_YEAR_FIELD);
    INSTALL_STATIC_INT(DateFormat, DAY_OF_WEEK_IN_MONTH_FIELD);
    INSTALL_STATIC_INT(DateFormat, WEEK_OF_YEAR_FIELD);
    INSTALL_STATIC_INT(DateFormat, WEEK_OF_MONTH_FIELD);
    INSTALL_STATIC_INT(DateFormat, AM_PM_FIELD);
    INSTALL_STATIC_INT(DateFormat, HOUR1_FIELD);
    INSTALL_STATIC_INT(DateFormat, HOUR0_FIELD);
    INSTALL_STATIC_INT(DateFormat, TIMEZONE_FIELD);
}
