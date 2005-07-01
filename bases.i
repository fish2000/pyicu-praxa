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

%module PyICU_bases

%{

#include "common.h"

%}

%include "common.i"

typedef enum {
    ULOC_ACTUAL_LOCALE = 0,
    ULOC_VALID_LOCALE  = 1,
} icu::ULocDataLocaleType;

#define U_FOLD_CASE_DEFAULT           0
#define U_FOLD_CASE_EXCLUDE_SPECIAL_I 1
#define U_COMPARE_CODE_POINT_ORDER    0x8000

namespace icu {

    class UMemory {
    public:
    };

    class UObject : public UMemory {
    public:
    };

}

%import "iterators.i"

namespace icu {

    class Replaceable : public UObject {
    public:
        int32_t length(void);
        UChar charAt(int32_t);
        UBool hasMetaData();
    };

    class UnicodeString : public Replaceable {
    public:
        UnicodeString();
        UnicodeString(UnicodeString &);
        UnicodeString(_PyString);
        UnicodeString(char *, _int32_t, _UConverter *, UErrorCode);
        UBool operator==(UnicodeString &);
        UBool operator!=(UnicodeString &);
        UBool operator>(UnicodeString &);
        UBool operator<(UnicodeString &);
        UBool operator>=(UnicodeString &);
        UBool operator<=(UnicodeString &);
        UBool operator==(_PyString);
        UBool operator!=(_PyString);
        UBool operator>(_PyString);
        UBool operator<(_PyString);
        UBool operator>=(_PyString);
        UBool operator<=(_PyString);
        UnicodeString0 &operator+=(UnicodeString &);
        UnicodeString0 &operator+=(_PyString);
        UnicodeString0 &append(UnicodeString &);
        UnicodeString0 &append(UnicodeString &, int32_t, int32_t);
        UnicodeString0 &append(_PyString);
        UnicodeString0 &append(_PyString, int32_t, int32_t);
        UnicodeString0 &append(UChar);

        int8_t compare(UnicodeString &);
        int8_t compare(_PyString);
        int8_t compare(int32_t, int32_t, UnicodeString &);
        int8_t compare(int32_t, int32_t, _PyString);

        int8_t compareBetween(int32_t, int32_t, UnicodeString &, int32_t, int32_t);
        int8_t compareBetween(int32_t, int32_t, _PyString, int32_t, int32_t);

        int8_t compareCodePointOrder(UnicodeString &);
        int8_t compareCodePointOrder(_PyString);
        int8_t compareCodePointOrder(int32_t, int32_t, UnicodeString &);
        int8_t compareCodePointOrder(int32_t, int32_t, _PyString);

        int8_t compareCodePointOrderBetween(int32_t, int32_t, UnicodeString &, int32_t, int32_t);
        int8_t compareCodePointOrderBetween(int32_t, int32_t, _PyString, int32_t, int32_t);

        int8_t caseCompare(UnicodeString &, uint32_t);
        int8_t caseCompare(_PyString, uint32_t);
        int8_t caseCompare(int32_t, int32_t, UnicodeString &, uint32_t);
        int8_t caseCompare(int32_t, int32_t, _PyString, uint32_t);

        int8_t caseCompareBetween(int32_t, int32_t, UnicodeString &, int32_t, int32_t, uint32_t);
        int8_t caseCompareBetween(int32_t, int32_t, _PyString, int32_t, int32_t, uint32_t);

	UBool startsWith(UnicodeString &);
	UBool startsWith(_PyString);
	UBool startsWith(UnicodeString &, int32_t, int32_t);
	UBool startsWith(_PyString, int32_t, int32_t);

	UBool endsWith(UnicodeString &);
	UBool endsWith(_PyString);
	UBool endsWith(UnicodeString &, int32_t, int32_t);
	UBool endsWith(_PyString, int32_t, int32_t);

	int32_t indexOf(UnicodeString &);
	int32_t indexOf(_PyString);
	int32_t indexOf(UnicodeString &, int32_t);
	int32_t indexOf(_PyString, int32_t);
	int32_t indexOf(UnicodeString &, int32_t, int32_t);
	int32_t indexOf(_PyString, int32_t, int32_t);
	int32_t indexOf(UnicodeString &, int32_t, int32_t, int32_t, int32_t);
	int32_t indexOf(_PyString, int32_t, int32_t, int32_t, int32_t);

	int32_t lastIndexOf(UnicodeString &);
	int32_t lastIndexOf(_PyString);
	int32_t lastIndexOf(UnicodeString &, int32_t);
	int32_t lastIndexOf(_PyString, int32_t);
	int32_t lastIndexOf(UnicodeString &, int32_t, int32_t);
	int32_t lastIndexOf(_PyString, int32_t, int32_t);
	int32_t lastIndexOf(UnicodeString &, int32_t, int32_t, int32_t, int32_t);
	int32_t lastIndexOf(_PyString, int32_t, int32_t, int32_t, int32_t);

	UnicodeString0 &trim();
	UnicodeString0 &reverse();
	UnicodeString0 &toUpper();
	UnicodeString0 &toUpper(Locale &);
	UnicodeString0 &toLower();
	UnicodeString0 &toLower(Locale &);
        UnicodeString0 &toTitle(BreakIterator *);
        UnicodeString0 &toTitle(BreakIterator *, icu::Locale &);
	UnicodeString0 &foldCase(uint32_t=0);

        %extend {
            UChar __getitem__(int32_t index)
            {
                if (index < 0)
                    index += self->length();

                return self->charAt(index);
            }

            int32_t __len__()
            {
                return self->length();
            }

            void __setitem__(int32_t index, UChar c)
            {
                if (index < 0)
                    index += self->length();

                self->setCharAt(index, c);
            }

            _UnicodeString *__getslice__(int32_t start, int32_t end)
            {
                if (start < 0)
                    start += self->length();
                if (end < 0)
                    end += self->length();

                UnicodeString *string = new UnicodeString();
                self->extractBetween(start, end, *string);

                return string;
            }

            void __setslice__(int32_t start, int32_t end, UnicodeString &string)
            {
                if (start < 0)
                    start += self->length();
                if (end < 0)
                    end += self->length();

                self->replaceBetween(start, end, string);
            }

            void __setslice__(int32_t start, int32_t end, _PyString string)
            {
                if (start < 0)
                    start += self->length();
                if (end < 0)
                    end += self->length();

                self->replaceBetween(start, end, string);
            }

            PyObject *__repr__()
            {
                PyObject *string = PyUnicode_FromUnicodeString(self);
                PyObject *format = PyString_FromString("<UnicodeString: %s>");
                PyObject *tuple = PyTuple_New(1);
                PyObject *repr;

                PyTuple_SET_ITEM(tuple, 0, string);
                repr = PyString_Format(format, tuple);
                Py_DECREF(format);
                Py_DECREF(tuple);

                return repr;
            }

            PyObject *__str__()
            {
                PyObject *string = PyUnicode_FromUnicodeString(self);
                PyObject *str = PyObject_Str(string);

                Py_DECREF(string);

                return str;
            }

            _PyString *__unicode__()
            {
                return self;
            }

	    int8_t __cmp__(UnicodeString &other)
            {
                return self->compare(other);
            }

	    int8_t __cmp__(_PyString other)
            {
                return self->compare(other);
            }
        }
    };

    class Formattable : public UObject {
    public:
        enum ISDATE {
            kIsDate
        };

        enum Type {
            kDate,
            kDouble,
            kLong,
            kString,
            kArray,
            kInt64,
            kObject
        };

        Formattable();
        Formattable(UDate, ISDATE);
        Formattable(double);
        Formattable(int32_t);
        Formattable(int64_t);
        Formattable(char *);
        Formattable(UnicodeString &);
        UBool operator==(Formattable &);
        UBool operator!=(Formattable &);
        Type getType();
        UBool isNumeric();
        double getDouble(UErrorCode);
        int32_t getLong(UErrorCode);
        int64_t getInt64(UErrorCode);
        UDate getDate(UErrorCode);
        UnicodeString1 &getString(UnicodeString &, UErrorCode);
        UnicodeString getString(_UnicodeString, UErrorCode);
        void setDouble(double);
        void setLong(int32_t);
        void setInt64(int64_t);
        void setDate(UDate);
        void setString(UnicodeString &);
        void setString(_PyString);
    };

    class Locale : public UObject {
    public:
        Locale();
        Locale(char *language, char *country=0, char *variant=0);
        char *getLanguage();
        char *getScript();
        char *getCountry();
        char *getVariant();
        char *getName();
        char *getBaseName();
        char *getISO3Language();
        char *getISO3Country();
        int32_t getLCID();
        UnicodeString1 &getDisplayLanguage(UnicodeString &);
        UnicodeString2 &getDisplayLanguage(Locale &, UnicodeString &);
        UnicodeString1 &getDisplayScript(UnicodeString &);
        UnicodeString2 &getDisplayScript(Locale &, UnicodeString &);
        UnicodeString1 &getDisplayCountry(UnicodeString &);
        UnicodeString2 &getDisplayCountry(Locale &, UnicodeString &);
        UnicodeString1 &getDisplayVariant(UnicodeString &);
        UnicodeString2 &getDisplayVariant(Locale &, UnicodeString &);
        UnicodeString1 &getDisplayName(UnicodeString &);
        UnicodeString2 &getDisplayName(Locale &, UnicodeString &);
        UnicodeString getDisplayLanguage(_UnicodeString);
        UnicodeString getDisplayLanguage(Locale &, _UnicodeString);
        UnicodeString getDisplayScript(_UnicodeString);
        UnicodeString getDisplayScript(Locale &, _UnicodeString);
        UnicodeString getDisplayCountry(_UnicodeString);
        UnicodeString getDisplayCountry(Locale &, _UnicodeString);
        UnicodeString getDisplayVariant(_UnicodeString);
        UnicodeString getDisplayVariant(Locale &, _UnicodeString);
        UnicodeString getDisplayName(_UnicodeString);
        UnicodeString getDisplayName(Locale &, _UnicodeString);

        static Locale getEnglish();
        static Locale getFrench();
        static Locale getGerman();
        static Locale getItalian();
        static Locale getJapanese();
        static Locale getKorean();
        static Locale getChinese();
        static Locale getSimplifiedChinese();
        static Locale getTraditionalChinese();
        static Locale getFrance();
        static Locale getGermany();
        static Locale getItaly();
        static Locale getJapan();
        static Locale getKorea();
        static Locale getChina();
        static Locale getPRC();
        static Locale getTaiwan();
        static Locale getUK();
        static Locale getUS();
        static Locale getCanada();
        static Locale getCanadaFrench();

        static Locale getDefault();
        static void setDefault(Locale &, UErrorCode);

	static Locale createFromName(char *);
	static Locale createCanonical(char *);

        static LocaleDict1 getAvailableLocales(_int32_t);

        %extend {
            PyObject *__repr__()
            {
                PyObject *string = PyString_FromString(self->getName());
                PyObject *format = PyString_FromString("<Locale: %s>");
                PyObject *tuple = PyTuple_New(1);
                PyObject *repr;

                PyTuple_SET_ITEM(tuple, 0, string);
                repr = PyString_Format(format, tuple);
                Py_DECREF(format);
                Py_DECREF(tuple);

                return repr;
            }

            static PyObject *getISOCountries()
            {
                const char *const *countries = Locale::getISOCountries();
                PyObject *list;
                int len = 0;

                while (countries[len] != NULL) len += 1;
                list = PyList_New(len);

                for (int i = 0; i < len; i++) {
                    PyObject *str = PyString_FromStringAndSize(countries[i], 2);
                    PyList_SET_ITEM(list, i, str);
                }

                return list;
            }

            static PyObject *getISOLanguages()
            {
                const char *const *languages = Locale::getISOLanguages();
                PyObject *list;
                int len = 0;

                while (languages[len] != NULL) len += 1;
                list = PyList_New(len);

                for (int i = 0; i < len; i++) {
                    PyObject *str = PyString_FromString(languages[i]);
                    PyList_SET_ITEM(list, i, str);
                }

                return list;
            }
        }
    };

    class MeasureUnit : public UObject {
    public:
        UBool operator==(UObject &);
    };

    class Measure : public UObject {
    public:
        UBool operator==(UObject &);
        Formattable getNumber();
    };

    class CurrencyUnit : public MeasureUnit {
    public:
        CurrencyUnit(ISO3Code, UErrorCode);
        ISO3Code getISOCurrency();

        %extend {
            PyObject *__repr__()
            {
                UnicodeString u(self->getISOCurrency());
                PyObject *string = PyUnicode_FromUnicodeString(&u);
                PyObject *format = PyString_FromString("<CurrencyUnit: %s>");
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

    class CurrencyAmount : public Measure {
    public:
        CurrencyAmount(Formattable &, ISO3Code, UErrorCode);
        CurrencyAmount(double, ISO3Code, UErrorCode);
        CurrencyUnit getCurrency();
        ISO3Code getISOCurrency();

        %extend {
            PyObject *__repr__()
            {
                Formattable number = self->getNumber();
                PyObject *amount = PyFloat_FromDouble(number.getDouble());

                UnicodeString u(self->getISOCurrency());
                PyObject *currency = PyUnicode_FromUnicodeString(&u);

                PyObject *format = PyString_FromString("<CurrencyAmount: %0.2f %s>");
                PyObject *tuple = PyTuple_New(2);
                PyObject *repr;

                PyTuple_SET_ITEM(tuple, 0, amount);
                PyTuple_SET_ITEM(tuple, 1, currency);
                repr = PyString_Format(format, tuple);
                Py_DECREF(format);
                Py_DECREF(tuple);

                return repr;
            }
        }
    };

    class StringEnumeration : public UObject {
    public:

        int32_t count(UErrorCode);
        void reset(UErrorCode);

        %extend {
            PyObject *next()
            {
                int32_t len;
                UErrorCode status = U_ZERO_ERROR;
                const char *str = self->next(&len, status);

                if (U_FAILURE(status))
                    throw ICUException(status);

                if (str == NULL)
                {
                    PyErr_SetNone(PyExc_StopIteration);
                    throw ICUException();
                }

                return PyString_FromStringAndSize(str, len);
            }

            PyObject *unext()
            {
                int32_t len;
                UErrorCode status = U_ZERO_ERROR;
                const UChar *str = self->unext(&len, status);

                if (U_FAILURE(status))
                    throw ICUException(status);

                if (str == NULL)
                {
                    PyErr_SetNone(PyExc_StopIteration);
                    throw ICUException();
                }

                UnicodeString u(str);
                return PyUnicode_FromUnicodeString(&u);
            }

            _UnicodeString *snext()
            {
                UErrorCode status = U_ZERO_ERROR;
                const UnicodeString *str = self->snext(status);

                if (U_FAILURE(status))
                    throw ICUException(status);

                if (str == NULL)
                {
                    PyErr_SetNone(PyExc_StopIteration);
                    throw ICUException();
                }

                return new UnicodeString(*str);
            }                

            StringEnumeration *__iter__()
            {
                return self;
            }
        }                
    };
}
