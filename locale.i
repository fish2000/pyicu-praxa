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

%module PyICU_locale

%{

#include "common.h"

%}

%include "common.i"
%import "bases.i"

typedef enum {
    ULOC_ACTUAL_LOCALE = 0,
    ULOC_VALID_LOCALE  = 1,
} icu::ULocDataLocaleType;

typedef enum {
    URES_NONE       = -1,
    URES_STRING     = 0,
    URES_BINARY     = 1,
    URES_TABLE      = 2,
    URES_ALIAS      = 3,
    URES_TABLE32    = 4,
    URES_INT        = 7,
    URES_ARRAY      = 8,
    URES_INT_VECTOR = 14,
    RES_RESERVED    = 15
} UResType;

namespace icu {

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

	_StringEnumeration *createKeywords(UErrorCode);

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

	    PyObject *getKeywordValue(char *name)
            {
                char buf[ULOC_FULLNAME_CAPACITY];
                UErrorCode status = U_ZERO_ERROR;
                int32_t len = self->getKeywordValue(name, buf, sizeof(buf) - 1,
                                                    status);

                if (len == 0)
                    Py_RETURN_NONE;

                return PyString_FromStringAndSize(buf, len);
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

    class ResourceBundle : public UObject {
    public:
        ResourceBundle(UnicodeString &, UErrorCode);
        ResourceBundle(UnicodeString &, Locale &, UErrorCode);
        ResourceBundle(_PyString, UErrorCode);
        ResourceBundle(_PyString, Locale &, UErrorCode);
        ResourceBundle(UErrorCode);

        int32_t getSize();
        UnicodeString getString(UErrorCode);
        uint32_t getUInt(UErrorCode);
        int32_t getInt(UErrorCode);

        char *getKey();
        char *getName();
        UResType getType();

        UBool hasNext();
        void resetIterator();
        ResourceBundle getNext(UErrorCode);
        UnicodeString getNextString(UErrorCode);
        ResourceBundle get(int32_t, UErrorCode);
        ResourceBundle get(char *, UErrorCode);
        ResourceBundle getWithFallback(char *, UErrorCode);
        UnicodeString getStringEx(int32_t, UErrorCode);
        UnicodeString getStringEx(char *, UErrorCode);

        char *getVersionNumber();

        %extend {
            PyObject *getBinary()
            {
                UErrorCode status = U_ZERO_ERROR;
                int32_t len;
                const uint8_t *data = self->getBinary(len, status);

                if (U_FAILURE(status))
                    throw ICUException(status);

                return PyString_FromStringAndSize((const char *) data, len);
            }

            PyObject *getIntVector()
            {
                UErrorCode status = U_ZERO_ERROR;
                int32_t len;
                const int32_t *data = self->getIntVector(len, status);

                if (U_FAILURE(status))
                    throw ICUException(status);

                PyObject *list = PyList_New(len);

                for (int i = 0; i < len; i++)
                    PyList_SET_ITEM(list, i, PyInt_FromLong(data[i]));

                return list;
            }

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
}
