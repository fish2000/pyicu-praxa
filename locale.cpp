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
#include "macros.h"

DECLARE_CONSTANTS_TYPE(ULocDataLocaleType);
DECLARE_CONSTANTS_TYPE(UResType);

/* Locale */

class t_locale : public _wrapper {
public:
    icu::Locale *object;
};

static int t_locale_init(t_locale *self, PyObject *args, PyObject *kwds);
static PyObject *t_locale_getLanguage(t_locale *self);
static PyObject *t_locale_getScript(t_locale *self);
static PyObject *t_locale_getCountry(t_locale *self);
static PyObject *t_locale_getVariant(t_locale *self);
static PyObject *t_locale_getName(t_locale *self);
static PyObject *t_locale_getBaseName(t_locale *self);
static PyObject *t_locale_getISO3Language(t_locale *self);
static PyObject *t_locale_getISO3Country(t_locale *self);
static PyObject *t_locale_getLCID(t_locale *self);
static PyObject *t_locale_getDisplayLanguage(t_locale *self, PyObject *args);
static PyObject *t_locale_getDisplayScript(t_locale *self, PyObject *args);
static PyObject *t_locale_getDisplayCountry(t_locale *self, PyObject *args);
static PyObject *t_locale_getDisplayVariant(t_locale *self, PyObject *args);
static PyObject *t_locale_getDisplayName(t_locale *self, PyObject *args);
static PyObject *t_locale_createKeywords(t_locale *self);
static PyObject *t_locale_getEnglish(PyTypeObject *type);
static PyObject *t_locale_getFrench(PyTypeObject *type);
static PyObject *t_locale_getGerman(PyTypeObject *type);
static PyObject *t_locale_getItalian(PyTypeObject *type);
static PyObject *t_locale_getJapanese(PyTypeObject *type);
static PyObject *t_locale_getKorean(PyTypeObject *type);
static PyObject *t_locale_getChinese(PyTypeObject *type);
static PyObject *t_locale_getSimplifiedChinese(PyTypeObject *type);
static PyObject *t_locale_getTraditionalChinese(PyTypeObject *type);
static PyObject *t_locale_getFrance(PyTypeObject *type);
static PyObject *t_locale_getGermany(PyTypeObject *type);
static PyObject *t_locale_getItaly(PyTypeObject *type);
static PyObject *t_locale_getJapan(PyTypeObject *type);
static PyObject *t_locale_getKorea(PyTypeObject *type);
static PyObject *t_locale_getChina(PyTypeObject *type);
static PyObject *t_locale_getPRC(PyTypeObject *type);
static PyObject *t_locale_getTaiwan(PyTypeObject *type);
static PyObject *t_locale_getUK(PyTypeObject *type);
static PyObject *t_locale_getUS(PyTypeObject *type);
static PyObject *t_locale_getCanada(PyTypeObject *type);
static PyObject *t_locale_getCanadaFrench(PyTypeObject *type);
static PyObject *t_locale_getDefault(PyTypeObject *type);
static PyObject *t_locale_setDefault(PyTypeObject *type, PyObject *args);
static PyObject *t_locale_createFromName(PyTypeObject *type, PyObject *args);
static PyObject *t_locale_createCanonical(PyTypeObject *type, PyObject *arg);
static PyObject *t_locale_getAvailableLocales(PyTypeObject *type);
static PyObject *t_locale_getKeywordValue(t_locale *self, PyObject *arg);
static PyObject *t_locale_getISOCountries(PyTypeObject *type);
static PyObject *t_locale_getISOLanguages(PyTypeObject *type);

static PyMethodDef t_locale_methods[] = {
    DECLARE_METHOD(t_locale, getLanguage, METH_NOARGS),
    DECLARE_METHOD(t_locale, getScript, METH_NOARGS),
    DECLARE_METHOD(t_locale, getCountry, METH_NOARGS),
    DECLARE_METHOD(t_locale, getVariant, METH_NOARGS),
    DECLARE_METHOD(t_locale, getName, METH_NOARGS),
    DECLARE_METHOD(t_locale, getBaseName, METH_NOARGS),
    DECLARE_METHOD(t_locale, getISO3Language, METH_NOARGS),
    DECLARE_METHOD(t_locale, getISO3Country, METH_NOARGS),
    DECLARE_METHOD(t_locale, getLCID, METH_NOARGS),
    DECLARE_METHOD(t_locale, getDisplayLanguage, METH_VARARGS),
    DECLARE_METHOD(t_locale, getDisplayScript, METH_VARARGS),
    DECLARE_METHOD(t_locale, getDisplayCountry, METH_VARARGS),
    DECLARE_METHOD(t_locale, getDisplayVariant, METH_VARARGS),
    DECLARE_METHOD(t_locale, getDisplayName, METH_VARARGS),
    DECLARE_METHOD(t_locale, createKeywords, METH_NOARGS),
    DECLARE_METHOD(t_locale, getEnglish, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getFrench, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getGerman, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getItalian, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getJapanese, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getKorean, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getChinese, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getSimplifiedChinese, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getTraditionalChinese, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getFrance, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getGermany, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getItaly, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getJapan, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getKorea, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getChina, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getPRC, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getTaiwan, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getUK, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getUS, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getCanada, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getCanadaFrench, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getDefault, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, setDefault, METH_VARARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, createFromName, METH_VARARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, createCanonical, METH_O | METH_CLASS),
    DECLARE_METHOD(t_locale, getAvailableLocales, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getKeywordValue, METH_O),
    DECLARE_METHOD(t_locale, getISOCountries, METH_NOARGS | METH_CLASS),
    DECLARE_METHOD(t_locale, getISOLanguages, METH_NOARGS | METH_CLASS),
    { NULL, NULL, 0, NULL }
};

DECLARE_TYPE(Locale, t_locale, UObject, icu::Locale, t_locale_init);

PyObject *wrap_Locale(const icu::Locale &locale)
{
    return wrap_Locale(new icu::Locale(locale), T_OWNED);
}

/* ResourceBundle */

class t_resourcebundle : public _wrapper {
public:
    icu::ResourceBundle *object;
};

static int t_resourcebundle_init(t_resourcebundle *self,
                                 PyObject *args, PyObject *kwds);
static PyObject *t_resourcebundle_getSize(t_resourcebundle *self);
static PyObject *t_resourcebundle_getString(t_resourcebundle *self,
                                            PyObject *args);
static PyObject *t_resourcebundle_getUInt(t_resourcebundle *self);
static PyObject *t_resourcebundle_getInt(t_resourcebundle *self);
static PyObject *t_resourcebundle_getKey(t_resourcebundle *self);
static PyObject *t_resourcebundle_getName(t_resourcebundle *self);
static PyObject *t_resourcebundle_getType(t_resourcebundle *self);
static PyObject *t_resourcebundle_hasNext(t_resourcebundle *self);
static PyObject *t_resourcebundle_resetIterator(t_resourcebundle *self);
static PyObject *t_resourcebundle_getNext(t_resourcebundle *self);
static PyObject *t_resourcebundle_getNextString(t_resourcebundle *self,
                                                PyObject *args);
static PyObject *t_resourcebundle_get(t_resourcebundle *self, PyObject *arg);
static PyObject *t_resourcebundle_getWithFallback(t_resourcebundle *self,
                                                  PyObject *arg);
static PyObject *t_resourcebundle_getStringEx(t_resourcebundle *self,
                                              PyObject *args);
static PyObject *t_resourcebundle_getVersionNumber(t_resourcebundle *self);
static PyObject *t_resourcebundle_getBinary(t_resourcebundle *self);
static PyObject *t_resourcebundle_getIntVector(t_resourcebundle *self);
static PyObject *t_resourcebundle_getLocale(t_resourcebundle *self,
                                            PyObject *args);

static PyMethodDef t_resourcebundle_methods[] = {
    DECLARE_METHOD(t_resourcebundle, getSize, METH_NOARGS),
    DECLARE_METHOD(t_resourcebundle, getString, METH_VARARGS),
    DECLARE_METHOD(t_resourcebundle, getUInt, METH_NOARGS),
    DECLARE_METHOD(t_resourcebundle, getInt, METH_NOARGS),
    DECLARE_METHOD(t_resourcebundle, getKey, METH_NOARGS),
    DECLARE_METHOD(t_resourcebundle, getName, METH_NOARGS),
    DECLARE_METHOD(t_resourcebundle, getType, METH_NOARGS),
    DECLARE_METHOD(t_resourcebundle, hasNext, METH_NOARGS),
    DECLARE_METHOD(t_resourcebundle, resetIterator, METH_NOARGS),
    DECLARE_METHOD(t_resourcebundle, getNext, METH_NOARGS),
    DECLARE_METHOD(t_resourcebundle, getNextString, METH_VARARGS),
    DECLARE_METHOD(t_resourcebundle, get, METH_O),
    DECLARE_METHOD(t_resourcebundle, getWithFallback, METH_O),
    DECLARE_METHOD(t_resourcebundle, getStringEx, METH_VARARGS),
    DECLARE_METHOD(t_resourcebundle, getVersionNumber, METH_NOARGS),
    DECLARE_METHOD(t_resourcebundle, getBinary, METH_NOARGS),
    DECLARE_METHOD(t_resourcebundle, getIntVector, METH_NOARGS),
    DECLARE_METHOD(t_resourcebundle, getLocale, METH_VARARGS),
    { NULL, NULL, 0, NULL }
};

DECLARE_TYPE(ResourceBundle, t_resourcebundle, UObject,
             icu::ResourceBundle, t_resourcebundle_init);

static PyObject *wrap_ResourceBundle(const icu::ResourceBundle &resourcebundle)
{
    return wrap_ResourceBundle(new icu::ResourceBundle(resourcebundle),
                               T_OWNED);
}

/* Locale */

static int t_locale_init(t_locale *self, PyObject *args, PyObject *kwds)
{
    char *language, *country, *variant;

    switch (PyTuple_Size(args)) {
      case 0:
        self->object = new icu::Locale();
        self->flags = T_OWNED;
        break;
      case 1:
        if (!parseArgs(args, "c", &language))
        {
            self->object = new icu::Locale(language);
            self->flags = T_OWNED;
            break;
        }
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
      case 2:
        if (!parseArgs(args, "cc", &language, &country))
        {
            self->object = new icu::Locale(language, country);
            self->flags = T_OWNED;
            break;
        }
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
      case 3:
        if (!parseArgs(args, "ccc", &language, &country, &variant))
        {
            self->object = new icu::Locale(language, country, variant);
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

static PyObject *t_locale_getLanguage(t_locale *self)
{
    return PyString_FromString(self->object->getLanguage());
}

static PyObject *t_locale_getScript(t_locale *self)
{
    return PyString_FromString(self->object->getScript());
}

static PyObject *t_locale_getCountry(t_locale *self)
{
    return PyString_FromString(self->object->getCountry());
}

static PyObject *t_locale_getVariant(t_locale *self)
{
    return PyString_FromString(self->object->getVariant());
}

static PyObject *t_locale_getName(t_locale *self)
{
    return PyString_FromString(self->object->getName());
}

static PyObject *t_locale_getBaseName(t_locale *self)
{
    return PyString_FromString(self->object->getBaseName());
}

static PyObject *t_locale_getISO3Language(t_locale *self)
{
    return PyString_FromString(self->object->getISO3Language());
}

static PyObject *t_locale_getISO3Country(t_locale *self)
{
    return PyString_FromString(self->object->getISO3Country());
}

static PyObject *t_locale_getLCID(t_locale *self)
{
    return PyInt_FromLong(self->object->getLCID());
}

static PyObject *t_locale_getDisplayLanguage(t_locale *self, PyObject *args)
{
    icu::Locale *locale;
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    switch (PyTuple_Size(args)) {
      case 0:
        self->object->getDisplayLanguage(_u);
        return PyUnicode_FromUnicodeString(&_u);
      case 1:
        if (!parseArgs(args, "P", TYPE_CLASSID(Locale), &locale))
        {
            self->object->getDisplayLanguage(*locale, _u);
            return PyUnicode_FromUnicodeString(&_u);
        }
        if (!parseArgs(args, "U", &u))
        {
            self->object->getDisplayLanguage(*u);
            Py_RETURN_ARG(args, 0);
        }
        break;
      case 2:
        if (!parseArgs(args, "PU", TYPE_CLASSID(Locale),
                       &locale, &u))
        {
            self->object->getDisplayLanguage(*locale, *u);
            Py_RETURN_ARG(args, 1);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "getDisplayLanguage", args);
}

static PyObject *t_locale_getDisplayScript(t_locale *self, PyObject *args)
{
    icu::Locale *locale;
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    switch (PyTuple_Size(args)) {
      case 0:
        self->object->getDisplayScript(_u);
        return PyUnicode_FromUnicodeString(&_u);
      case 1:
        if (!parseArgs(args, "P", TYPE_CLASSID(Locale), &locale))
        {
            self->object->getDisplayScript(*locale, _u);
            return PyUnicode_FromUnicodeString(&_u);
        }
        if (!parseArgs(args, "U", &u))
        {
            self->object->getDisplayScript(*u);
            Py_RETURN_ARG(args, 0);
        }
        break;
      case 2:
        if (!parseArgs(args, "PU", TYPE_CLASSID(Locale),
                       &locale, &u))
        {
            self->object->getDisplayScript(*locale, *u);
            Py_RETURN_ARG(args, 1);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "getDisplayScript", args);
}

static PyObject *t_locale_getDisplayCountry(t_locale *self, PyObject *args)
{
    icu::Locale *locale;
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    switch (PyTuple_Size(args)) {
      case 0:
        self->object->getDisplayCountry(_u);
        return PyUnicode_FromUnicodeString(&_u);
      case 1:
        if (!parseArgs(args, "P", TYPE_CLASSID(Locale), &locale))
        {
            self->object->getDisplayCountry(*locale, _u);
            return PyUnicode_FromUnicodeString(&_u);
        }
        if (!parseArgs(args, "U", &u))
        {
            self->object->getDisplayCountry(*u);
            Py_RETURN_ARG(args, 0);
        }
        break;
      case 2:
        if (!parseArgs(args, "PU", TYPE_CLASSID(Locale),
                       &locale, &u))
        {
            self->object->getDisplayCountry(*locale, *u);
            Py_RETURN_ARG(args, 1);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "getDisplayCountry", args);
}

static PyObject *t_locale_getDisplayVariant(t_locale *self, PyObject *args)
{
    icu::Locale *locale;
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    switch (PyTuple_Size(args)) {
      case 0:
        self->object->getDisplayVariant(_u);
        return PyUnicode_FromUnicodeString(&_u);
      case 1:
        if (!parseArgs(args, "P", TYPE_CLASSID(Locale), &locale))
        {
            self->object->getDisplayVariant(*locale, _u);
            return PyUnicode_FromUnicodeString(&_u);
        }
        if (!parseArgs(args, "U", &u))
        {
            self->object->getDisplayVariant(*u);
            Py_RETURN_ARG(args, 0);
        }
        break;
      case 2:
        if (!parseArgs(args, "PU", TYPE_CLASSID(Locale),
                       &locale, &u))
        {
            self->object->getDisplayVariant(*locale, *u);
            Py_RETURN_ARG(args, 1);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "getDisplayVariant", args);
}

static PyObject *t_locale_getDisplayName(t_locale *self, PyObject *args)
{
    icu::Locale *locale;
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    switch (PyTuple_Size(args)) {
      case 0:
        self->object->getDisplayName(_u);
        return PyUnicode_FromUnicodeString(&_u);
      case 1:
        if (!parseArgs(args, "P", TYPE_CLASSID(Locale), &locale))
        {
            self->object->getDisplayName(*locale, _u);
            return PyUnicode_FromUnicodeString(&_u);
        }
        if (!parseArgs(args, "U", &u))
        {
            self->object->getDisplayName(*u);
            Py_RETURN_ARG(args, 0);
        }
        break;
      case 2:
        if (!parseArgs(args, "PU", TYPE_CLASSID(Locale), &locale, &u))
        {
            self->object->getDisplayName(*locale, *u);
            Py_RETURN_ARG(args, 1);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "getDisplayName", args);
}

static PyObject *t_locale_createKeywords(t_locale *self)
{
    icu::StringEnumeration *se;
    STATUS_CALL(se = self->object->createKeywords(status));

    return wrap_StringEnumeration(se, T_OWNED);
}

static PyObject *t_locale_getEnglish(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getEnglish());
}

static PyObject *t_locale_getFrench(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getFrench());
}

static PyObject *t_locale_getGerman(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getGerman());
}

static PyObject *t_locale_getItalian(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getItalian());
}

static PyObject *t_locale_getJapanese(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getJapanese());
}

static PyObject *t_locale_getKorean(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getKorean());
}

static PyObject *t_locale_getChinese(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getChinese());
}

static PyObject *t_locale_getSimplifiedChinese(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getSimplifiedChinese());
}

static PyObject *t_locale_getTraditionalChinese(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getTraditionalChinese());
}

static PyObject *t_locale_getFrance(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getFrance());
}

static PyObject *t_locale_getGermany(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getGermany());
}

static PyObject *t_locale_getItaly(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getItaly());
}

static PyObject *t_locale_getJapan(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getJapan());
}

static PyObject *t_locale_getKorea(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getKorea());
}

static PyObject *t_locale_getChina(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getChina());
}

static PyObject *t_locale_getPRC(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getPRC());
}

static PyObject *t_locale_getTaiwan(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getTaiwan());
}

static PyObject *t_locale_getUK(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getUK());
}

static PyObject *t_locale_getUS(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getUS());
}

static PyObject *t_locale_getCanada(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getCanada());
}

static PyObject *t_locale_getCanadaFrench(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getCanadaFrench());
}

static PyObject *t_locale_getDefault(PyTypeObject *self)
{
    return wrap_Locale(icu::Locale::getDefault());
}

static PyObject *t_locale_setDefault(PyTypeObject *type, PyObject *args)
{
    icu::Locale *locale;

    switch (PyTuple_Size(args)) {
      case 0:
        STATUS_CALL(icu::Locale::setDefault(NULL, status));
        Py_RETURN_NONE;
      case 1:
        if (!parseArgs(args, "P", TYPE_CLASSID(Locale), &locale))
        {
            STATUS_CALL(icu::Locale::setDefault(*locale, status));
            Py_RETURN_NONE;
        }
        break;
    }

    return PyErr_SetArgsError(type, "setDefault", args);
}

static PyObject *t_locale_createFromName(PyTypeObject *type, PyObject *args)
{
    icu::Locale locale;
    char *name;

    switch (PyTuple_Size(args)) {
      case 0:
        locale = icu::Locale::createFromName(NULL);
        return wrap_Locale(locale);
      case 1:
        if (!parseArgs(args, "c", &name))
        {
            locale = icu::Locale::createFromName(name);
            return wrap_Locale(locale);
        }
        break;
    }

    return PyErr_SetArgsError(type, "createFromName", args);
}

static PyObject *t_locale_createCanonical(PyTypeObject *type, PyObject *arg)
{
    icu::Locale locale;
    char *name;

    if (!parseArg(arg, "c", &name))
    {
        locale = icu::Locale::createCanonical(name);
        return wrap_Locale(locale);
    }

    return PyErr_SetArgsError(type, "createCanonical", arg);
}

static PyObject *t_locale_getAvailableLocales(PyTypeObject *type)
{
    int count;
    const icu::Locale *locales = icu::Locale::getAvailableLocales(count);
    PyObject *dict = PyDict_New();

    for (int32_t i = 0; i < count; i++) {
        Locale *locale = (Locale *) locales + i;
        PyObject *obj = wrap_Locale(locale, 0);
        PyDict_SetItemString(dict, locale->getName(), obj);
	Py_DECREF(obj);
    }

    return dict;
}

static PyObject *t_locale_getKeywordValue(t_locale *self, PyObject *arg)
{
    char *name;

    if (!parseArg(arg, "c", &name))
    {
        char buf[ULOC_FULLNAME_CAPACITY];
        UErrorCode status = U_ZERO_ERROR;
        int32_t len = self->object->getKeywordValue(name, buf, sizeof(buf) - 1,
                                                    status);

        if (len == 0)
            Py_RETURN_NONE;

        return PyString_FromStringAndSize(buf, len);
    }

    return PyErr_SetArgsError((PyObject *) self, "getKeywordValue", arg);
}

static PyObject *t_locale_getISOCountries(PyTypeObject *type)
{
    const char *const *countries = icu::Locale::getISOCountries();
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

static PyObject *t_locale_getISOLanguages(PyTypeObject *type)
{
    const char *const *languages = icu::Locale::getISOLanguages();
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

static PyObject *t_locale_str(t_locale *self)
{
    return PyString_FromString(self->object->getName());
}


/* ResourceBundle */

static int t_resourcebundle_init(t_resourcebundle *self,
                                 PyObject *args, PyObject *kwds)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;
    icu::Locale *locale;
    icu::ResourceBundle *bundle;

    switch (PyTuple_Size(args)) {
      case 0:
        INT_STATUS_CALL(bundle = new icu::ResourceBundle(status));
        self->object = bundle;
        self->flags = T_OWNED;
        break;
      case 1:
        if (!parseArgs(args, "S", &u, &_u))
        {
            INT_STATUS_CALL(bundle = new icu::ResourceBundle(*u, status));
            self->object = bundle;
            self->flags = T_OWNED;
            break;
        }
      case 2:
        if (!parseArgs(args, "SP", TYPE_CLASSID(Locale),
                       &u, &_u, &locale))
        {
            INT_STATUS_CALL(bundle = new icu::ResourceBundle(*u, *locale,
                                                             status));
            self->object = bundle;
            self->flags = T_OWNED;
            break;
        }
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
    }
        
    if (self->object)
        return 0;

    return -1;
}

static PyObject *t_resourcebundle_getSize(t_resourcebundle *self)
{
    return PyInt_FromLong(self->object->getSize());
}

static PyObject *t_resourcebundle_getString(t_resourcebundle *self,
                                            PyObject *args)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    switch (PyTuple_Size(args)) {
      case 0:
        STATUS_CALL(_u = self->object->getString(status));
        return PyUnicode_FromUnicodeString(&_u);
      case 1:
        if (!parseArgs(args, "U", &u))
        {
            STATUS_CALL(u->setTo(self->object->getString(status)));
            Py_RETURN_ARG(args, 0);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "getString", args);
}

static PyObject *t_resourcebundle_getUInt(t_resourcebundle *self)
{
    int n;

    STATUS_CALL(n = self->object->getUInt(status));
    return PyInt_FromLong(n);
}

static PyObject *t_resourcebundle_getInt(t_resourcebundle *self)
{
    int n;

    STATUS_CALL(n = self->object->getInt(status));
    return PyInt_FromLong(n);
}

static PyObject *t_resourcebundle_getKey(t_resourcebundle *self)
{
    return PyString_FromString(self->object->getKey());
}

static PyObject *t_resourcebundle_getName(t_resourcebundle *self)
{
    return PyString_FromString(self->object->getName());
}

static PyObject *t_resourcebundle_getType(t_resourcebundle *self)
{
    return PyInt_FromLong((long) self->object->getType());
}

static PyObject *t_resourcebundle_hasNext(t_resourcebundle *self)
{
    int b = self->object->hasNext();
    Py_RETURN_BOOL(b);
}

static PyObject *t_resourcebundle_resetIterator(t_resourcebundle *self)
{
    self->object->resetIterator();
    Py_RETURN_NONE;
}

static PyObject *t_resourcebundle_getNext(t_resourcebundle *self)
{
    STATUS_CALL(return wrap_ResourceBundle(self->object->getNext(status)));
}

static PyObject *t_resourcebundle_getNextString(t_resourcebundle *self,
                                                PyObject *args)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;

    switch (PyTuple_Size(args)) {
      case 0:
        STATUS_CALL(_u = self->object->getNextString(status));
        return PyUnicode_FromUnicodeString(&_u);
      case 1:
        if (!parseArgs(args, "U", &u))
        {
            STATUS_CALL(u->setTo(self->object->getNextString(status)));
            Py_RETURN_ARG(args, 0);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "getNextString", args);
}

static PyObject *t_resourcebundle_get(t_resourcebundle *self, PyObject *arg)
{
    int i;
    char *key;

    if (!parseArg(arg, "i", &i))
        STATUS_CALL(return wrap_ResourceBundle(self->object->get(i, status)));

    if (!parseArg(arg, "c", &key))
        STATUS_CALL(return wrap_ResourceBundle(self->object->get(key, status)));

    return PyErr_SetArgsError((PyObject *) self, "get", arg);
}

static PyObject *t_resourcebundle_getWithFallback(t_resourcebundle *self,
                                                  PyObject *arg)
{
    char *key;

    if (!parseArg(arg, "c", &key))
        STATUS_CALL(return wrap_ResourceBundle(self->object->getWithFallback(key, status)));

    return PyErr_SetArgsError((PyObject *) self, "getWithFallback", arg);
}

static PyObject *t_resourcebundle_getStringEx(t_resourcebundle *self,
                                              PyObject *args)
{
    icu::UnicodeString *u;
    icu::UnicodeString _u;
    char *key;
    int i;

    switch (PyTuple_Size(args)) {
      case 1:
        if (!parseArgs(args, "i", &i))
        {
            STATUS_CALL(_u = self->object->getStringEx(i, status));
            return PyUnicode_FromUnicodeString(&_u);
        }
        if (!parseArgs(args, "c", &key))
        {
            STATUS_CALL(_u = self->object->getStringEx(key, status));
            return PyUnicode_FromUnicodeString(&_u);
        }
      case 2:
        if (!parseArgs(args, "iU", &i, &u))
        {
            STATUS_CALL(u->setTo(self->object->getStringEx(i, status)));
            Py_RETURN_ARG(args, 1);
        }
        if (!parseArgs(args, "cU", &i, &u))
        {
            STATUS_CALL(u->setTo(self->object->getStringEx(i, status)));
            Py_RETURN_ARG(args, 1);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "getStringEx", args);
}

static PyObject *t_resourcebundle_getVersionNumber(t_resourcebundle *self)
{
    return PyString_FromString(self->object->getVersionNumber());
}

static PyObject *t_resourcebundle_getBinary(t_resourcebundle *self)
{
    int32_t len;
    const uint8_t *data;

    STATUS_CALL(data = self->object->getBinary(len, status));

    return PyString_FromStringAndSize((const char *) data, len);
}

static PyObject *t_resourcebundle_getIntVector(t_resourcebundle *self)
{
    int32_t len;
    const int32_t *data;

    STATUS_CALL(data = self->object->getIntVector(len, status));

    PyObject *list = PyList_New(len);

    for (int i = 0; i < len; i++)
        PyList_SET_ITEM(list, i, PyInt_FromLong(data[i]));

    return list;
}

static PyObject *t_resourcebundle_getLocale(t_resourcebundle *self,
                                            PyObject *args)
{
    ULocDataLocaleType type;

    switch (PyTuple_Size(args)) {
      case 0:
        return wrap_Locale(self->object->getLocale());
      case 1:
        if (!parseArgs(args, "i", &type))
        {
            icu::Locale locale;

            STATUS_CALL(locale = self->object->getLocale(type, status));
            return wrap_Locale(locale);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "getLocale", args);
} 

static PyObject *t_resourcebundle_iter(t_resourcebundle *self)
{
    self->object->resetIterator();

    Py_INCREF(self);
    return (PyObject *) self;
}

static PyObject *t_resourcebundle_next(t_resourcebundle *self)
{
    if (self->object->hasNext())
        STATUS_CALL(return wrap_ResourceBundle(self->object->getNext(status)));

    PyErr_SetNone(PyExc_StopIteration);
    return NULL;
}

static PyObject *t_resourcebundle_str(t_resourcebundle *self)
{
    switch (self->object->getType()) {
      case URES_NONE:
        return PyString_FromString("URES_NONE");
      case URES_STRING:
        return PyString_FromString("URES_STRING");
      case URES_BINARY:
        return PyString_FromString("URES_BINARY");
      case URES_TABLE:
        return PyString_FromString("URES_TABLE");
      case URES_ALIAS:
        return PyString_FromString("URES_ALIAS");
      case URES_TABLE32:
        return PyString_FromString("URES_TABLE32");
      case URES_ARRAY:
        return PyString_FromString("URES_ARRAY");
      case URES_INT_VECTOR:
        return PyString_FromString("URES_INT_VECTOR");
      case RES_RESERVED:
        return PyString_FromString("RES_RESERVED");
      default:
        return PyString_FromString("unknown");
    }
}

void _init_locale(PyObject *m)
{
    LocaleType.tp_str = (reprfunc) t_locale_str;
    ResourceBundleType.tp_iter = (getiterfunc) t_resourcebundle_iter;
    ResourceBundleType.tp_iternext = (iternextfunc) t_resourcebundle_next;
    ResourceBundleType.tp_str = (reprfunc) t_resourcebundle_str;

    INSTALL_CONSTANTS_TYPE(ULocDataLocaleType, m);
    INSTALL_CONSTANTS_TYPE(UResType, m);
    REGISTER_TYPE(Locale, m);
    REGISTER_TYPE(ResourceBundle, m);

    INSTALL_ENUM(ULocDataLocaleType, ULOC_ACTUAL_LOCALE);
    INSTALL_ENUM(ULocDataLocaleType, ULOC_VALID_LOCALE);

    INSTALL_ENUM(UResType, URES_NONE);
    INSTALL_ENUM(UResType, URES_STRING);
    INSTALL_ENUM(UResType, URES_BINARY);
    INSTALL_ENUM(UResType, URES_TABLE);
    INSTALL_ENUM(UResType, URES_ALIAS);
    INSTALL_ENUM(UResType, URES_TABLE32);
    INSTALL_ENUM(UResType, URES_INT);
    INSTALL_ENUM(UResType, URES_ARRAY);
    INSTALL_ENUM(UResType, URES_INT_VECTOR);
    INSTALL_ENUM(UResType, RES_RESERVED);
}
