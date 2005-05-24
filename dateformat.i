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

%module PyICU_dateformat

%{

#include "common.h"

%}

%include "common.i"
%import "bases.i"
%import "format.i"
%import "numberformat.i"
%import "calendar.i"

namespace icu {

    class DateFormatSymbols : public UObject {
    public:
        DateFormatSymbols(UErrorCode);
        DateFormatSymbols(Locale &, UErrorCode);
        DateFormatSymbols(char *, UErrorCode);
        DateFormatSymbols(Locale &, char *, UErrorCode);
        
        UBool operator==(DateFormatSymbols &);
        UBool operator!=(DateFormatSymbols &);

        UnicodeStringArray2 getEras(_int32_t);
        void setEras(LeakyUnicodeStringArray3, _int32_t);

        UnicodeStringArray2 getMonths(_int32_t);
        void setMonths(LeakyUnicodeStringArray3, _int32_t);
        UnicodeStringArray2 getShortMonths(_int32_t);
        void setShortMonths(LeakyUnicodeStringArray3, _int32_t);

        UnicodeStringArray2 getWeekdays(_int32_t);
        void setWeekdays(LeakyUnicodeStringArray3, _int32_t);
        UnicodeStringArray2 getShortWeekdays(_int32_t);
        void setShortWeekdays(LeakyUnicodeStringArray3, _int32_t);

        UnicodeStringArray2 getAmPmStrings(_int32_t);
        void setAmPmStrings(LeakyUnicodeStringArray3, _int32_t);

        UnicodeString1 &getLocalPatternChars(UnicodeString &);
        UnicodeString getLocalPatternChars(_UnicodeString);
        void setLocalPatternChars(UnicodeString &);
        void setLocalPatternChars(_PyString);

        %extend {
            const Locale getLocale(ULocDataLocaleType type=ULOC_VALID_LOCALE)
            {
                UErrorCode status = U_ZERO_ERROR;
                Locale locale = self->getLocale(type, status);

                if (U_FAILURE(status))
                    throw ICUException(status);

                return locale;
            }
        }
    };

    class DateFormat : public Format {
    public:
        enum EStyle {
            kNone       = -1,
            kFull       = 0,
            kLong       = 1,
            kMedium     = 2,
            kShort      = 3,
            kDateOffset = 4,
            kDateTime   = 8,
            kDefault    = kMedium,
            FULL        = kFull,
            LONG        = kLong,
            MEDIUM      = kMedium,
            SHORT       = kShort,
            DEFAULT     = kDefault,
            DATE_OFFSET = kDateOffset,
            NONE        = kNone,
            DATE_TIME   = kDateTime,
        };

        enum EField {
            kEraField = UDAT_ERA_FIELD,
            kYearField = UDAT_YEAR_FIELD,
            kMonthField = UDAT_MONTH_FIELD,
            kDateField = UDAT_DATE_FIELD,
            kHourOfDay1Field = UDAT_HOUR_OF_DAY1_FIELD,
            kHourOfDay0Field = UDAT_HOUR_OF_DAY0_FIELD,
            kMinuteField = UDAT_MINUTE_FIELD,
            kSecondField = UDAT_SECOND_FIELD,
            kMillisecondField = UDAT_FRACTIONAL_SECOND_FIELD,
            kDayOfWeekField = UDAT_DAY_OF_WEEK_FIELD,
            kDayOfYearField = UDAT_DAY_OF_YEAR_FIELD,
            kDayOfWeekInMonthField = UDAT_DAY_OF_WEEK_IN_MONTH_FIELD,
            kWeekOfYearField = UDAT_WEEK_OF_YEAR_FIELD,
            kWeekOfMonthField = UDAT_WEEK_OF_MONTH_FIELD,
            kAmPmField = UDAT_AM_PM_FIELD,
            kHour1Field = UDAT_HOUR1_FIELD,
            kHour0Field = UDAT_HOUR0_FIELD,
            kTimezoneField = UDAT_TIMEZONE_FIELD,
            kYearWOYField = UDAT_YEAR_WOY_FIELD,
            kDOWLocalField = UDAT_DOW_LOCAL_FIELD,
            kExtendedYearField = UDAT_EXTENDED_YEAR_FIELD,
            kJulianDayField = UDAT_JULIAN_DAY_FIELD,
            kMillisecondsInDayField = UDAT_MILLISECONDS_IN_DAY_FIELD,
            ERA_FIELD = UDAT_ERA_FIELD,
            YEAR_FIELD = UDAT_YEAR_FIELD,
            MONTH_FIELD = UDAT_MONTH_FIELD,
            DATE_FIELD = UDAT_DATE_FIELD,
            HOUR_OF_DAY1_FIELD = UDAT_HOUR_OF_DAY1_FIELD,
            HOUR_OF_DAY0_FIELD = UDAT_HOUR_OF_DAY0_FIELD,
            MINUTE_FIELD = UDAT_MINUTE_FIELD,
            SECOND_FIELD = UDAT_SECOND_FIELD,
            MILLISECOND_FIELD = UDAT_FRACTIONAL_SECOND_FIELD,
            DAY_OF_WEEK_FIELD = UDAT_DAY_OF_WEEK_FIELD,
            DAY_OF_YEAR_FIELD = UDAT_DAY_OF_YEAR_FIELD,
            DAY_OF_WEEK_IN_MONTH_FIELD = UDAT_DAY_OF_WEEK_IN_MONTH_FIELD,
            WEEK_OF_YEAR_FIELD = UDAT_WEEK_OF_YEAR_FIELD,
            WEEK_OF_MONTH_FIELD = UDAT_WEEK_OF_MONTH_FIELD,
            AM_PM_FIELD = UDAT_AM_PM_FIELD,
            HOUR1_FIELD = UDAT_HOUR1_FIELD,
            HOUR0_FIELD = UDAT_HOUR0_FIELD,
            TIMEZONE_FIELD = UDAT_TIMEZONE_FIELD
        };

        UBool isLenient(void);
        void setLenient(UBool);
        UnicodeString2 &format(Formattable &, UnicodeString &, UErrorCode);
        UnicodeString2 &format(Formattable &, UnicodeString &, FieldPosition &, UErrorCode);
        UnicodeString format(Formattable &, _UnicodeString, UErrorCode);
        UnicodeString format(Formattable &, _UnicodeString, FieldPosition &, UErrorCode);
        UnicodeString2 &format(UDate, UnicodeString &);
        UnicodeString2 &format(UDate, UnicodeString &, FieldPosition &);
        UnicodeString format(UDate, _UnicodeString);
        UnicodeString format(UDate, _UnicodeString, FieldPosition &);
        UDate parse(UnicodeString &, UErrorCode);
        UDate parse(UnicodeString &, ParsePosition &);
        UDate parse(_PyString, UErrorCode);
        UDate parse(_PyString, ParsePosition &);

        Calendar *getCalendar();
        void setCalendar(Calendar &);
        NumberFormat *getNumberFormat();
        void setNumberFormat(NumberFormat &);
        const_TimeZone &getTimeZone();
        void setTimeZone(TimeZone &);

        static _DateFormat *createInstance();
        static _DateFormat *createTimeInstance(EStyle=kDefault,
                                               Locale=Locale::getDefault());
        static _DateFormat *createDateInstance(EStyle=kDefault,
                                               Locale=Locale::getDefault());
        static _DateFormat *createDateTimeInstance(EStyle=kDefault,
                                                   EStyle=kDefault,
                                                   Locale=Locale::getDefault());
        static LocaleArray1 getAvailableLocales(_int32_t);
    };

    class SimpleDateFormat : public DateFormat {
    public:
        SimpleDateFormat(UErrorCode);
        SimpleDateFormat(UnicodeString &, UErrorCode);
        SimpleDateFormat(UnicodeString &, Locale &, UErrorCode);
        SimpleDateFormat(UnicodeString &, DateFormatSymbols &, UErrorCode);
        SimpleDateFormat(_PyString, UErrorCode);
        SimpleDateFormat(_PyString, Locale &, UErrorCode);
        SimpleDateFormat(_PyString, DateFormatSymbols &, UErrorCode);
        UnicodeString1 &toPattern(UnicodeString &);
        UnicodeString toPattern(_UnicodeString);
        UnicodeString1 &toLocalizedPattern(UnicodeString &, UErrorCode);
        UnicodeString toLocalizedPattern(_UnicodeString, UErrorCode);
        void applyPattern(UnicodeString &);
        void applyPattern(_PyString);
        void applyLocalizedPattern(UnicodeString &, UErrorCode);
        void applyLocalizedPattern(_PyString, UErrorCode);
        void set2DigitYearStart(UDate, UErrorCode);
        UDate get2DigitYearStart(UErrorCode);

        DateFormatSymbols *getDateFormatSymbols();
        void setDateFormatSymbols(DateFormatSymbols &);

        %extend {
            PyObject *__repr__()
            {
                UnicodeString u; self->toPattern(u);
                PyObject *string = PyUnicode_FromUnicodeString(&u);
                PyObject *format = PyString_FromString("<SimpleDateFormat: %s>");
                PyObject *tuple = PyTuple_New(1);
                PyObject *repr;

                PyTuple_SET_ITEM(tuple, 0, string);
                repr = PyString_Format(format, tuple);
                Py_DECREF(format);
                Py_DECREF(tuple);

                return repr;
            }
        }
    };
}
