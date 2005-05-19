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

%module PyICU_calendar

%{

#include "common.h"
#include <unicode/datefmt.h>

static PyObject *repr(char *name, Calendar *self)
{
    UErrorCode status = U_ZERO_ERROR;
    UDate date = self->getTime(status);
    UnicodeString u = UnicodeString();

    if (U_SUCCESS(status))
    {
        Locale locale = self->getLocale(ULOC_VALID_LOCALE, status);
        DateFormat *df = DateFormat::createDateTimeInstance(DateFormat::kDefault, DateFormat::kDefault, locale);

        u = df->format(date, u);
        delete df;
    }

    PyObject *string = PyUnicode_FromUnicodeString(&u);
    PyObject *pyname = PyString_FromString(name);
    PyObject *format = PyString_FromString("<%s: %s>");
    PyObject *tuple = PyTuple_New(2);
    PyObject *repr;

    PyTuple_SET_ITEM(tuple, 0, pyname);
    PyTuple_SET_ITEM(tuple, 1, string);
    repr = PyString_Format(format, tuple);
    Py_DECREF(format);
    Py_DECREF(tuple);

    return repr;
}

%}

%include "common.i"
%import "bases.i"
%import "string.i"
%import "locale.i"

enum UCalendarDateFields {
    UCAL_ERA,
    UCAL_YEAR,
    UCAL_MONTH,
    UCAL_WEEK_OF_YEAR,
    UCAL_WEEK_OF_MONTH,
    UCAL_DATE,
    UCAL_DAY_OF_YEAR,
    UCAL_DAY_OF_WEEK,
    UCAL_DAY_OF_WEEK_IN_MONTH,
    UCAL_AM_PM,
    UCAL_HOUR,
    UCAL_HOUR_OF_DAY,
    UCAL_MINUTE,
    UCAL_SECOND,
    UCAL_MILLISECOND,
    UCAL_ZONE_OFFSET,
    UCAL_DST_OFFSET,
    UCAL_YEAR_WOY,
    UCAL_DOW_LOCAL,
    UCAL_EXTENDED_YEAR,       
    UCAL_JULIAN_DAY, 
    UCAL_MILLISECONDS_IN_DAY,
    UCAL_DAY_OF_MONTH=UCAL_DATE
};

enum UCalendarDaysOfWeek {
    UCAL_SUNDAY = 1,
    UCAL_MONDAY,
    UCAL_TUESDAY,
    UCAL_WEDNESDAY,
    UCAL_THURSDAY,
    UCAL_FRIDAY,
    UCAL_SATURDAY
};

enum UCalendarMonths {
    UCAL_JANUARY,
    UCAL_FEBRUARY,
    UCAL_MARCH,
    UCAL_APRIL,
    UCAL_MAY,
    UCAL_JUNE,
    UCAL_JULY,
    UCAL_AUGUST,
    UCAL_SEPTEMBER,
    UCAL_OCTOBER,
    UCAL_NOVEMBER,
    UCAL_DECEMBER,
    UCAL_UNDECIMBER
};

enum UCalendarAMPMs {
    UCAL_AM,
    UCAL_PM
};

namespace icu {

    class Calendar : public UObject {
    public:
        enum EDateFields {
            ERA,
            YEAR,
            MONTH,
            WEEK_OF_YEAR,
            WEEK_OF_MONTH,
            DATE,
            DAY_OF_YEAR,
            DAY_OF_WEEK,
            DAY_OF_WEEK_IN_MONTH,
            AM_PM,
            HOUR,
            HOUR_OF_DAY,
            MINUTE,
            SECOND,
            MILLISECOND,
            ZONE_OFFSET,
            DST_OFFSET,
            YEAR_WOY,
            DOW_LOCAL
        };
        enum EDaysOfWeek {
            SUNDAY = 1,
            MONDAY,
            TUESDAY,
            WEDNESDAY,
            THURSDAY,
            FRIDAY,
            SATURDAY
        };
        enum EMonths {
            JANUARY,
            FEBRUARY,
            MARCH,
            APRIL,
            MAY,
            JUNE,
            JULY,
            AUGUST,
            SEPTEMBER,
            OCTOBER,
            NOVEMBER,
            DECEMBER,
            UNDECIMBER
        };
        enum EAmpm {
            AM,
            PM
        };

        UBool operator==(Calendar &);
        UBool operator!=(Calendar &);

        UDate getTime(UErrorCode);
        void setTime(UDate, UErrorCode);

        UBool isEquivalentTo(Calendar &);

        UBool equals(Calendar &, UErrorCode);
        UBool before(Calendar &, UErrorCode);
        UBool after(Calendar &, UErrorCode);

        void add(UCalendarDateFields, int32_t, UErrorCode);
        void roll(UCalendarDateFields, UBool, UErrorCode);
        void roll(UCalendarDateFields, int32_t, UErrorCode);
        
        int32_t fieldDifference(UDate, UCalendarDateFields, UErrorCode);
        
        UBool inDaylightTime(UErrorCode);

        void setLenient(UBool);
        UBool isLenient();

        void setFirstDayOfWeek(UCalendarDaysOfWeek);
        UCalendarDaysOfWeek getFirstDayOfWeek(UErrorCode);

        void setMinimalDaysInFirstWeek(uint8_t);
        int32_t getMinimum(UCalendarDateFields);
        int32_t getMaximum(UCalendarDateFields);
        int32_t getGreatestMinimum(UCalendarDateFields);
        int32_t getLeastMaximum(UCalendarDateFields);
        int32_t getActualMinimum(UCalendarDateFields, UErrorCode);
        int32_t getActualMaximum(UCalendarDateFields, UErrorCode);
        int32_t get(UCalendarDateFields, UErrorCode);
        
        UBool isSet(UCalendarDateFields);
        void set(UCalendarDateFields, int32_t);
        void set(int32_t, int32_t, int32_t);
        void set(int32_t, int32_t, int32_t, int32_t, int32_t);
        void set(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);

        void clear();
        void clear(UCalendarDateFields);
        
        UBool haveDefaultCentury();
        UDate defaultCenturyStart();
        int32_t defaultCenturyStartYear();

        static _Calendar *createInstance(UErrorCode);
        static _Calendar *createInstance(Locale &, UErrorCode);
        static LocaleArray1 getAvailableLocales(_int32_t);
        static UDate getNow();

        %extend {
            const Locale getLocale(ULocDataLocaleType type=ULOC_VALID_LOCALE)
            {
                UErrorCode status = U_ZERO_ERROR;
                Locale locale = self->getLocale(type, status);

                if (U_FAILURE(status))
                    throw ICUException(status, "error");

                return locale;
            }

            const char *getLocaleID(ULocDataLocaleType type=ULOC_VALID_LOCALE)
            {
                UErrorCode status = U_ZERO_ERROR;
                const char *localeID = self->getLocaleID(type, status);

                if (U_FAILURE(status))
                    throw ICUException(status, "error");

                return localeID;
            }

            PyObject *__repr__()
            {
                return repr("Calendar", self);
            }
        }
    };

    class GregorianCalendar : public Calendar {
    public:
        enum EEras {
            BC,
            AD
        };
        
        GregorianCalendar(UErrorCode);
        GregorianCalendar(Locale &, UErrorCode);
        GregorianCalendar(int32_t, int32_t, int32_t, UErrorCode);
        GregorianCalendar(int32_t, int32_t, int32_t, int32_t, int32_t, UErrorCode);
        GregorianCalendar(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, UErrorCode);

        void setGregorianChange(UDate, UErrorCode);
        UDate getGregorianChange();

        UBool isLeapYear(int32_t);
        UBool isEquivalentTo(Calendar &);

        %extend {
            PyObject *__repr__()
            {
                return repr("GregorianCalendar", self);
            }
        };
    };
}
