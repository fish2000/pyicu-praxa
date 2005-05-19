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

%module PyICU_numberformat

%{

#include "common.h"

%}

%include "common.i"
%import "bases.i"
%import "format.i"

namespace icu {

    class DecimalFormatSymbols : public UObject {
    public:
        enum ENumberFormatSymbol {
            kDecimalSeparatorSymbol,
            kGroupingSeparatorSymbol,
            kPatternSeparatorSymbol,
            kPercentSymbol,
            kZeroDigitSymbol,
            kDigitSymbol,
            kMinusSignSymbol,
            kPlusSignSymbol,
            kCurrencySymbol,
            kIntlCurrencySymbol,
            kMonetarySeparatorSymbol,
            kExponentialSymbol,
            kPerMillSymbol,
            kPadEscapeSymbol,
            kInfinitySymbol,
            kNaNSymbol,
            kSignificantDigitSymbol,
        };

        DecimalFormatSymbols(UErrorCode);
        DecimalFormatSymbols(Locale &, UErrorCode);

        UBool operator==(DecimalFormatSymbols &);
        UBool operator!=(DecimalFormatSymbols &);

        UnicodeString getSymbol(ENumberFormatSymbol);
        void setSymbol(ENumberFormatSymbol, UnicodeString &);
        void setSymbol(ENumberFormatSymbol, _PyString);

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

    class NumberFormat : public Format {
    public:
        enum EAlignmentFields {
            kIntegerField,
            kFractionField,
            INTEGER_FIELD  = kIntegerField,
            FRACTION_FIELD = kFractionField
        };

        UnicodeString2 &format(Formattable &, UnicodeString &, UErrorCode);
        UnicodeString2 &format(Formattable &, UnicodeString &, FieldPosition &, UErrorCode);
        UnicodeString format(Formattable &, _UnicodeString, UErrorCode);
        UnicodeString format(Formattable &, _UnicodeString, FieldPosition &, UErrorCode);

        UnicodeString2 &format(double, UnicodeString &);
        UnicodeString2 &format(int32_t, UnicodeString &);
        UnicodeString2 &format(int64_t, UnicodeString &);
        UnicodeString2 &format(double, UnicodeString &, FieldPosition &);
        UnicodeString2 &format(int32_t, UnicodeString &, FieldPosition &);
        UnicodeString2 &format(int64_t, UnicodeString &, FieldPosition &);

        UnicodeString format(double, _UnicodeString);
        UnicodeString format(int32_t, _UnicodeString);
        UnicodeString format(int64_t, _UnicodeString);
        UnicodeString format(double, _UnicodeString, FieldPosition &);
        UnicodeString format(int32_t, _UnicodeString, FieldPosition &);
        UnicodeString format(int64_t, _UnicodeString, FieldPosition &);

        void parse(UnicodeString &, Formattable &, ParsePosition &);
        void parse(UnicodeString &, Formattable &, UErrorCode);

        Formattable2 &parseCurrency(UnicodeString &, Formattable &, ParsePosition &);
        Formattable2 &parseCurrency(_PyString, Formattable &, ParsePosition &);

        UBool isParseIntegerOnly();
        void setParseIntegerOnly(UBool);
        UBool isGroupingUsed();
        void setGroupingUsed(UBool);

        int32_t getMaximumIntegerDigits();
        void setMaximumIntegerDigits(int32_t);
        int32_t getMinimumIntegerDigits();
        void setMinimumIntegerDigits(int32_t);

        int32_t getMaximumFractionDigits();
        void setMaximumFractionDigits(int32_t);
        int32_t getMinimumFractionDigits();
        void setMinimumFractionDigits(int32_t);

        void setCurrency(ISO3Code, UErrorCode);
        ISO3Code getCurrency();

        static _NumberFormat *createInstance(UErrorCode);
        static _NumberFormat *createInstance(Locale &, UErrorCode);
        static _NumberFormat *createCurrencyInstance(UErrorCode);
        static _NumberFormat *createCurrencyInstance(Locale &, UErrorCode);
        static _NumberFormat *createPercentInstance(UErrorCode);
        static _NumberFormat *createPercentInstance(Locale &, UErrorCode);
        static _NumberFormat *createScientificInstance(UErrorCode);
        static _NumberFormat *createScientificInstance(Locale &, UErrorCode);

	static LocaleArray1 getAvailableLocales(_int32_t);

        %extend {

            Formattable parse(_PyString string, ParsePosition &pos)
            {
                Formattable number;
                self->parse(string, number, pos);

                return number;
            }

            Formattable parse(_PyString string, UErrorCode status)
            {
                Formattable number;
                self->parse(string, number, status);

                if (U_FAILURE(status))
                    throw ICUException(status);

                return number;
            }
        }
    };

    class DecimalFormat : public NumberFormat {
    public:
        enum ERoundingMode {
            kRoundCeiling,  
            kRoundFloor,    
            kRoundDown,     
            kRoundUp,       
            kRoundHalfEven, 
            kRoundHalfDown, 
            kRoundHalfUp    
        };

        enum EPadPosition {
            kPadBeforePrefix,
            kPadAfterPrefix,
            kPadBeforeSuffix,
            kPadAfterSuffix
        };

        DecimalFormat(UErrorCode);
        DecimalFormat(UnicodeString &, UErrorCode);
        DecimalFormat(_PyString, UErrorCode);

        UnicodeString1 &getPositivePrefix(UnicodeString &);
        void setPositivePrefix(UnicodeString &);
        UnicodeString getPositivePrefix(_UnicodeString);
        void setPositivePrefix(_PyString);

        UnicodeString1 &getNegativePrefix(UnicodeString &);
        void setNegativePrefix(UnicodeString &);
        UnicodeString getNegativePrefix(_UnicodeString);
        void setNegativePrefix(_PyString);

        int32_t getMultiplier();
        void setMultiplier(int32_t);

        double getRoundingIncrement();
        void setRoundingIncrement(double);

        ERoundingMode getRoundingMode();
        void setRoundingMode(ERoundingMode);

        int32_t getFormatWidth();
        void setFormatWidth(int32_t);
        
        UnicodeString getPadCharacterString();
        void setPadCharacter(UnicodeString &);
        void setPadCharacter(_UnicodeString);

        EPadPosition getPadPosition();
        void setPadPosition(EPadPosition);
        
        UBool isScientificNotation();
        void setScientificNotation(UBool);

        int8_t getMinimumExponentDigits();
        void setMinimumExponentDigits(int8_t);

        UBool isExponentSignAlwaysShown();
        void setExponentSignAlwaysShown(UBool);
        UBool isDecimalSeparatorAlwaysShown();
        void setDecimalSeparatorAlwaysShown(UBool);

        int32_t getGroupingSize();
        void setGroupingSize(int32_t);
        int32_t getSecondaryGroupingSize();
        void setSecondaryGroupingSize(int32_t);

        UnicodeString1 &toPattern(UnicodeString &);
        UnicodeString toPattern(_UnicodeString);
        UnicodeString1 &toLocalizedPattern(UnicodeString &);
        UnicodeString toLocalizedPattern(_UnicodeString);

        void applyPattern(UnicodeString &, UErrorCode);
        void applyPattern(_PyString, UErrorCode);
        void applyLocalizedPattern(UnicodeString &, UErrorCode);
        void applyLocalizedPattern(_PyString, UErrorCode);

        int32_t getMaximumSignificantDigits();
        void setMaximumSignificantDigits(int32_t);
        int32_t getMinimumSignificantDigits();
        void setMinimumSignificantDigits(int32_t);

        UBool areSignificantDigitsUsed();
        void setSignificantDigitsUsed(UBool);

        %extend {
            PyObject *__repr__()
            {
                UnicodeString u; self->toPattern(u);
                PyObject *string = PyUnicode_FromUnicodeString(&u);
                PyObject *format = PyString_FromString("<DecimalFormat: %s>");
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

    class RuleBasedNumberFormat : public NumberFormat {
    public:
        RuleBasedNumberFormat(UnicodeString &, UParseError, UErrorCode);
        RuleBasedNumberFormat(UnicodeString &, Locale &, UParseError, UErrorCode);
        RuleBasedNumberFormat(UnicodeString &, UnicodeString &, UParseError, UErrorCode);
        RuleBasedNumberFormat(UnicodeString &, UnicodeString &, Locale &, UParseError, UErrorCode);
        RuleBasedNumberFormat(_PyString, UParseError, UErrorCode);
        RuleBasedNumberFormat(_PyString, Locale &, UParseError, UErrorCode);
        RuleBasedNumberFormat(_PyString, _PyString, UParseError, UErrorCode);
        RuleBasedNumberFormat(_PyString, _PyString, Locale &, UParseError, UErrorCode);

        UnicodeString getRules();
        int32_t getNumberOfRuleSetNames();
        UnicodeString getRuleSetName(int32_t);
        int32_t getNumberOfRuleSetDisplayNameLocales();
        Locale getRuleSetDisplayNameLocale(int32_t, UErrorCode);
        UnicodeString getRuleSetDisplayName(int32_t, Locale &=Locale::getDefault());
        UnicodeString getRuleSetDisplayName(UnicodeString &, Locale &=Locale::getDefault());
        UnicodeString getRuleSetDisplayName(_PyString, Locale &=Locale::getDefault());

        UnicodeString2 &format(Formattable &, UnicodeString &, UErrorCode);
        UnicodeString2 &format(Formattable &, UnicodeString &, FieldPosition &, UErrorCode);
        UnicodeString format(Formattable &, _UnicodeString, UErrorCode);
        UnicodeString format(Formattable &, _UnicodeString, FieldPosition &, UErrorCode);

        UnicodeString2 &format(double, UnicodeString &);
        UnicodeString2 &format(int32_t, UnicodeString &);
        UnicodeString2 &format(double, UnicodeString &, FieldPosition &);
        UnicodeString2 &format(int32_t, UnicodeString &, FieldPosition &);
        UnicodeString2 &format(int64_t, UnicodeString &, FieldPosition &);

        UnicodeString2 &format(double, UnicodeString &, UnicodeString &, FieldPosition &, UErrorCode);
        UnicodeString2 &format(int32_t, UnicodeString &, UnicodeString &, FieldPosition &, UErrorCode);
        UnicodeString2 &format(int64_t, UnicodeString &, UnicodeString &, FieldPosition &, UErrorCode);

        UnicodeString format(double, _UnicodeString);
        UnicodeString format(int32_t, _UnicodeString);
        UnicodeString format(double, _UnicodeString, FieldPosition &);
        UnicodeString format(int32_t, _UnicodeString, FieldPosition &);
        UnicodeString format(int64_t, _UnicodeString, FieldPosition &);

        UnicodeString format(double, _UnicodeString, _PyString, FieldPosition &, UErrorCode);
        UnicodeString format(int32_t, _UnicodeString, _PyString, FieldPosition &, UErrorCode);
        UnicodeString format(int64_t, _UnicodeString, _PyString, FieldPosition &, UErrorCode);

        void setLenient(UBool);
        UBool isLenient();

        void setDefaultRuleSet(UnicodeString &, UErrorCode);
        void setDefaultRuleSet(_PyString, UErrorCode);
        UnicodeString getDefaultRuleSetName();

        %extend {
            PyObject *__repr__()
            {
                UnicodeString u = self->getRules();
                PyObject *string = PyUnicode_FromUnicodeString(&u);
                PyObject *format = PyString_FromString("<RuleBasedNumberFormat: %s>");
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

    class ChoiceFormat : public NumberFormat {
    public:
        ChoiceFormat(UnicodeString &, UErrorCode);
        ChoiceFormat(_PyString, UErrorCode);
        
        void applyPattern(UnicodeString &, UParseError, UErrorCode);
        void applyPattern(_PyString, UParseError, UErrorCode);

        UnicodeString1 &toPattern(UnicodeString &);
        UnicodeString toPattern(_UnicodeString);
    };
}
