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

static PyObject *tz_repr(char *name, TimeZone *self)
{
    UnicodeString u; self->getID(u);
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

static PyObject *cal_repr(char *name, Calendar *self)
{
    UErrorCode status = U_ZERO_ERROR;
    UDate date = self->getTime(status);
    UnicodeString u;

    if (U_SUCCESS(status))
    {
        Locale locale = self->getLocale(ULOC_VALID_LOCALE, status);
        DateFormat *df = DateFormat::createDateTimeInstance(DateFormat::kDefault, DateFormat::kDefault, locale);

        df->format(date, u);
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

    class TimeZone : public UObject {
    public:

        enum EDisplayType {
            SHORT = 1,
            LONG
        };

        UBool operator==(TimeZone &);
        UBool operator!=(TimeZone &);

        int32_t getOffset(uint8_t, int32_t, int32_t, int32_t, uint8_t, int32_t, UErrorCode);
        int32_t getOffset(uint8_t, int32_t, int32_t, int32_t, uint8_t, int32_t, int32_t, UErrorCode);

        void setRawOffset(int32_t);
        int32_t getRawOffset();

        UnicodeString1 &getID(UnicodeString &);
        UnicodeString getID(_UnicodeString);
        void setID(UnicodeString &);
        void setID(_PyString);

        UnicodeString1 &getDisplayName(UnicodeString &);
        UnicodeString2 &getDisplayName(Locale &, UnicodeString &);
        UnicodeString getDisplayName(_UnicodeString);
        UnicodeString getDisplayName(Locale &, _UnicodeString);
        UnicodeString3 &getDisplayName(UBool, EDisplayType, UnicodeString &);
        UnicodeString getDisplayName(UBool, EDisplayType, _UnicodeString);

        UBool useDaylightTime();
        UBool inDaylightTime(UDate, UErrorCode);
        UBool hasSameRules(TimeZone &);

        static _TimeZone *getGMT();
        static _TimeZone *createTimeZone(UnicodeString &);
        static _TimeZone *createTimeZone(_PyString);
        static _StringEnumeration *createEnumeration();
        static _StringEnumeration *createEnumeration(int32_t);
        static _StringEnumeration *createEnumeration(char *);
        static int32_t countEquivalentIDs(UnicodeString &);
        static int32_t countEquivalentIDs(_PyString);
        static UnicodeString getEquivalentID(UnicodeString &, int32_t);
        static UnicodeString getEquivalentID(_PyString, int32_t);
        static _TimeZone *createDefault();
        static void adoptDefault(TimeZone_ *);

        %extend {
            PyObject *__repr__()
            {
                return tz_repr("TimeZone", self);
            }

            PyObject *getOffset(UDate date, UBool local)
            {
                int32_t rawOffset, dstOffset;
                UErrorCode status = U_ZERO_ERROR;

                self->getOffset(date, local, rawOffset, dstOffset, status);
                if (U_FAILURE(status))
                    throw ICUException(status);
                    
                PyObject *tuple = PyTuple_New(2);
                PyTuple_SET_ITEM(tuple, 0, PyInt_FromLong(rawOffset));
                PyTuple_SET_ITEM(tuple, 1, PyInt_FromLong(dstOffset));

                return tuple;
            }
        }
    };
    
    class SimpleTimeZone : public TimeZone {
    public:
        enum TimeMode {
            WALL_TIME = 0,
            STANDARD_TIME,
            UTC_TIME
        };

        SimpleTimeZone(int32_t, UnicodeString &);
        SimpleTimeZone(int32_t, _PyString);
        SimpleTimeZone(int32_t, UnicodeString &, int8_t, int8_t, int8_t, int32_t, int8_t, int8_t, int8_t, int32_t, UErrorCode);
        SimpleTimeZone(int32_t, UnicodeString &, int8_t, int8_t, int8_t, int32_t, int8_t, int8_t, int8_t, int32_t, int32_t, UErrorCode);
 	SimpleTimeZone(int32_t, UnicodeString &, int8_t, int8_t, int8_t, int32_t, TimeMode, int8_t, int8_t, int8_t, int32_t, TimeMode, int32_t, UErrorCode);

        void setStartYear(int32_t year);
        void setStartRule(int32_t, int32_t, int32_t, int32_t, UErrorCode);
        //void setStartRule(int32_t, int32_t, int32_t, int32_t, TimeMode, UErrorCode);
        void setStartRule(int32_t, int32_t, int32_t, UErrorCode);
        //void setStartRule(int32_t, int32_t, int32_t, TimeMode, UErrorCode);
        void setStartRule(int32_t, int32_t, int32_t, int32_t, UBool, UErrorCode);
        void setStartRule(int32_t, int32_t, int32_t, int32_t, TimeMode, UBool, UErrorCode);
        void setEndRule(int32_t, int32_t, int32_t, int32_t, UErrorCode);
        void setEndRule(int32_t, int32_t, int32_t, int32_t, TimeMode, UErrorCode);
        void setEndRule(int32_t, int32_t, int32_t, UErrorCode);
        //void setEndRule(int32_t, int32_t, int32_t, TimeMode, UErrorCode);
        void setEndRule(int32_t, int32_t, int32_t, int32_t, UBool, UErrorCode);
        void setEndRule(int32_t, int32_t, int32_t, int32_t, TimeMode, UBool, UErrorCode);

        int32_t getOffset(uint8_t, int32_t, int32_t, int32_t, uint8_t, int32_t, UErrorCode);
        int32_t getOffset(uint8_t, int32_t, int32_t, int32_t, uint8_t, int32_t, int32_t, UErrorCode);
        int32_t getOffset(uint8_t, int32_t, int32_t, int32_t, uint8_t, int32_t, int32_t, int32_t, UErrorCode);

        int32_t getRawOffset();
        void setRawOffset(int32_t);

        void setDSTSavings(int32_t, UErrorCode);
        int32_t getDSTSavings();

        %extend {
            PyObject *__repr__()
            {
                return tz_repr("SimpleTimeZone", self);
            }

            PyObject *getOffset(UDate date, UBool local)
            {
                int32_t rawOffset, dstOffset;
                UErrorCode status = U_ZERO_ERROR;

                self->getOffset(date, local, rawOffset, dstOffset, status);
                if (U_FAILURE(status))
                    throw ICUException(status);
                    
                PyObject *tuple = PyTuple_New(2);
                PyTuple_SET_ITEM(tuple, 0, PyInt_FromLong(rawOffset));
                PyTuple_SET_ITEM(tuple, 1, PyInt_FromLong(dstOffset));

                return tuple;
            }
        }
    };

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

        void setTimeZone(TimeZone &);
        const_TimeZone &getTimeZone();
        
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
        static _Calendar *createInstance(TimeZone &, UErrorCode);
        static _Calendar *createInstance(TimeZone &, Locale &, UErrorCode);
        static LocaleArray1 getAvailableLocales(_int32_t);
        static UDate getNow();

        %extend {
            const Locale getLocale(ULocDataLocaleType type=ULOC_VALID_LOCALE)
            {
                UErrorCode status = U_ZERO_ERROR;
                Locale locale = self->getLocale(type, status);

                if (U_FAILURE(status))
                    throw ICUException(status);

                return locale;
            }

            const char *getLocaleID(ULocDataLocaleType type=ULOC_VALID_LOCALE)
            {
                UErrorCode status = U_ZERO_ERROR;
                const char *localeID = self->getLocaleID(type, status);

                if (U_FAILURE(status))
                    throw ICUException(status);

                return localeID;
            }

            PyObject *__repr__()
            {
                return cal_repr("Calendar", self);
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
                return cal_repr("GregorianCalendar", self);
            }
        };
    };
}


%pythoncode {

    from datetime import tzinfo, timedelta

    class ICUtzinfo(tzinfo):

        instances = {}

        def getInstance(cls, id):
            try:
                return cls.instances[id]
            except KeyError:
                instance = cls(TimeZone.createTimeZone(id))
                cls.instances[id] = instance
                return instance
        getInstance = classmethod(getInstance)

        def __init__(self, timezone):
            if not isinstance(timezone, TimeZone):
                raise TypeError, timezone
            super(ICUtzinfo, self).__init__()
            self.timezone = timezone

        def __repr__(self):
            return "<ICUtzinfo: %s>" %(self.timezone.getID())

        def __str__(self):
            return str(self.timezone.getID())

        def _notzsecs(self, dt):
            return ((dt.toordinal() - 719163) * 86400.0 +
                    dt.hour * 3600.0 + dt.minute * 60.0 +
                    float(dt.second) + dt.microsecond / 1e6)

        def utcoffset(self, dt):
            raw, dst = self.timezone.getOffset(self._notzsecs(dt), True)
            return timedelta(seconds = (raw + dst) / 1000)

        def dst(self, dt):
            raw, dst = self.timezone.getOffset(self._notzsecs(dt), True)
            return timedelta(seconds = dst / 1000)

        def tzname(self, dt):
            return str(self.timezone.getID())
}
