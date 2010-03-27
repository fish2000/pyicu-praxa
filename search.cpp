/* ====================================================================
 * Copyright (c) 2004-2010 Open Source Applications Foundation.
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
#include "collator.h"
#include "iterators.h"
#include "search.h"
#include "macros.h"

DECLARE_CONSTANTS_TYPE(USearchAttribute);
DECLARE_CONSTANTS_TYPE(USearchAttributeValue);


/* SearchIterator */

class t_searchiterator : public _wrapper {
public:
    SearchIterator *object;
    PyObject *iterator;
};

static PyObject *t_searchiterator_getOffset(t_searchiterator *self);
static PyObject *t_searchiterator_setOffset(t_searchiterator *self,
                                            PyObject *arg);
static PyObject *t_searchiterator_getAttribute(t_searchiterator *self,
                                               PyObject *arg);
static PyObject *t_searchiterator_setAttribute(t_searchiterator *self,
                                               PyObject *args);
static PyObject *t_searchiterator_getMatchedStart(t_searchiterator *self);
static PyObject *t_searchiterator_getMatchedLength(t_searchiterator *self);
static PyObject *t_searchiterator_getMatchedText(t_searchiterator *self,
                                                 PyObject *args);
static PyObject *t_searchiterator_getBreakIterator(t_searchiterator *self);
static PyObject *t_searchiterator_setBreakIterator(t_searchiterator *self,
                                                   PyObject *arg);
static PyObject *t_searchiterator_getText(t_searchiterator *self);
static PyObject *t_searchiterator_setText(t_searchiterator *self,
                                          PyObject *arg);
static PyObject *t_searchiterator_first(t_searchiterator *self);
static PyObject *t_searchiterator_last(t_searchiterator *self);
static PyObject *t_searchiterator_next(t_searchiterator *self);
static PyObject *t_searchiterator_following(t_searchiterator *self,
                                            PyObject *arg);
static PyObject *t_searchiterator_preceding(t_searchiterator *self,
                                            PyObject *arg);
static PyObject *t_searchiterator_reset(t_searchiterator *self);

static PyMethodDef t_searchiterator_methods[] = {
    DECLARE_METHOD(t_searchiterator, getOffset, METH_NOARGS),
    DECLARE_METHOD(t_searchiterator, setOffset, METH_O),
    DECLARE_METHOD(t_searchiterator, getAttribute, METH_O),
    DECLARE_METHOD(t_searchiterator, setAttribute, METH_VARARGS),
    DECLARE_METHOD(t_searchiterator, getMatchedStart, METH_NOARGS),
    DECLARE_METHOD(t_searchiterator, getMatchedLength, METH_NOARGS),
    DECLARE_METHOD(t_searchiterator, getMatchedText, METH_VARARGS),
    DECLARE_METHOD(t_searchiterator, getBreakIterator, METH_NOARGS),
    DECLARE_METHOD(t_searchiterator, setBreakIterator, METH_O),
    DECLARE_METHOD(t_searchiterator, getText, METH_NOARGS),
    DECLARE_METHOD(t_searchiterator, setText, METH_O),
    DECLARE_METHOD(t_searchiterator, first, METH_NOARGS),
    DECLARE_METHOD(t_searchiterator, last, METH_NOARGS),
    DECLARE_METHOD(t_searchiterator, next, METH_NOARGS),
    DECLARE_METHOD(t_searchiterator, following, METH_O),
    DECLARE_METHOD(t_searchiterator, preceding, METH_O),
    DECLARE_METHOD(t_searchiterator, reset, METH_NOARGS),
    { NULL, NULL, 0, NULL }
};

DECLARE_TYPE(SearchIterator, t_searchiterator, UObject, SearchIterator,
             abstract_init);

/* StringSearch */

class t_stringsearch : public _wrapper {
public:
    StringSearch *object;
    PyObject *iterator;
    PyObject *collator;
};

static int t_stringsearch_init(t_stringsearch *self,
                               PyObject *args, PyObject *kwds);
static PyObject *t_stringsearch_getCollator(t_stringsearch *self);
static PyObject *t_stringsearch_setCollator(t_stringsearch *self,
                                            PyObject *arg);
static PyObject *t_stringsearch_getPattern(t_stringsearch *self);
static PyObject *t_stringsearch_setPattern(t_stringsearch *self, PyObject *arg);

static PyMethodDef t_stringsearch_methods[] = {
    DECLARE_METHOD(t_stringsearch, getCollator, METH_NOARGS),
    DECLARE_METHOD(t_stringsearch, setCollator, METH_O),
    DECLARE_METHOD(t_stringsearch, getPattern, METH_NOARGS),
    DECLARE_METHOD(t_stringsearch, setPattern, METH_O),
    { NULL, NULL, 0, NULL }
};

DECLARE_TYPE(StringSearch, t_stringsearch, SearchIterator, StringSearch,
             t_stringsearch_init);


/* SearchIterator */

static void t_searchiterator_dealloc(t_searchiterator *self)
{
    if (self->object)
    {
        if (self->flags & T_OWNED)
            delete self->object;
            
        self->object = NULL;

        Py_XDECREF(self->iterator); self->iterator = NULL;
    }

    self->ob_type->tp_free((PyObject *) self);
}

static PyObject *t_searchiterator_getOffset(t_searchiterator *self)
{
    int32_t offset = self->object->getOffset();
    return PyInt_FromLong(offset);
}

static PyObject *t_searchiterator_setOffset(t_searchiterator *self,
                                            PyObject *arg)
{
    int32_t offset;

    if (!parseArg(arg, "i", &offset))
    {
        STATUS_CALL(self->object->setOffset(offset, status));
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setOffset", arg);
}

static PyObject *t_searchiterator_getAttribute(t_searchiterator *self,
                                               PyObject *arg)
{
    USearchAttribute attribute;

    if (!parseArg(arg, "i", &attribute))
    {
        USearchAttributeValue value = self->object->getAttribute(attribute);
        return PyInt_FromLong(value);
    }

    return PyErr_SetArgsError((PyObject *) self, "getAttribute", arg);
}

static PyObject *t_searchiterator_setAttribute(t_searchiterator *self,
                                               PyObject *args)
{
    USearchAttribute attribute;
    USearchAttributeValue value;

    if (!parseArgs(args, "ii", &attribute, &value))
    {
        STATUS_CALL(self->object->setAttribute(attribute, value, status));
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setAttribute", args);
}

static PyObject *t_searchiterator_getMatchedStart(t_searchiterator *self)
{
    int32_t start = self->object->getMatchedStart();
    return PyInt_FromLong(start);
}

static PyObject *t_searchiterator_getMatchedLength(t_searchiterator *self)
{
    int32_t length = self->object->getMatchedLength();
    return PyInt_FromLong(length);
}

static PyObject *t_searchiterator_getMatchedText(t_searchiterator *self,
                                                 PyObject *args)
{
    UnicodeString *u, _u;

    switch (PyTuple_Size(args)) {
      case 0:
        self->object->getMatchedText(_u);
        return PyUnicode_FromUnicodeString(&_u);
      case 1:
        if (!parseArgs(args, "U", &u))
        {
            self->object->getMatchedText(*u);
            Py_RETURN_ARG(args, 0);
        }
        break;
    }

    return PyErr_SetArgsError((PyObject *) self, "getMatchedText", args);
}

static PyObject *t_searchiterator_getBreakIterator(t_searchiterator *self)
{
    if (self->iterator)
    {
        Py_INCREF(self->iterator);
        return self->iterator;
    }

    Py_RETURN_NONE;
}

static PyObject *t_searchiterator_setBreakIterator(t_searchiterator *self,
                                                   PyObject *arg)
{
    BreakIterator *iterator;

    if (arg == Py_None)
    {
        STATUS_CALL(self->object->setBreakIterator(NULL, status));
        Py_XDECREF(self->iterator); self->iterator = NULL;
        Py_RETURN_NONE;
    }
    if (!parseArg(arg, "P", TYPE_ID(BreakIterator), &iterator))
    {
        STATUS_CALL(self->object->setBreakIterator(iterator, status));
        Py_INCREF(arg); Py_XDECREF(self->iterator); self->iterator = arg;
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setBreakIterator", arg);
}

static PyObject *t_searchiterator_getText(t_searchiterator *self)
{
    UnicodeString u = self->object->getText();
    return PyUnicode_FromUnicodeString(&u);
}

static PyObject *t_searchiterator_setText(t_searchiterator *self, PyObject *arg)
{
    UnicodeString *u, _u;
    CharacterIterator *chars;

    if (!parseArg(arg, "S", &u, &_u))
    {
        STATUS_CALL(self->object->setText(*u, status));
        Py_RETURN_NONE;
    }
    else if (!parseArg(arg, "P", TYPE_ID(CharacterIterator), &chars))
    {
        STATUS_CALL(self->object->setText(*chars, status));
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setText", arg);
}

static PyObject *t_searchiterator_first(t_searchiterator *self)
{
    int32_t first;
    STATUS_CALL(first = self->object->first(status));

    return PyInt_FromLong(first);
}

static PyObject *t_searchiterator_last(t_searchiterator *self)
{
    int32_t last;
    STATUS_CALL(last = self->object->last(status));

    return PyInt_FromLong(last);
}

static PyObject *t_searchiterator_next(t_searchiterator *self)
{
    int32_t next;
    STATUS_CALL(next = self->object->next(status));

    return PyInt_FromLong(next);
}

static PyObject *t_searchiterator_following(t_searchiterator *self,
                                            PyObject *arg)
{
    int32_t position, following;

    if (!parseArg(arg, "i", &position))
    {
        STATUS_CALL(following = self->object->following(position, status));
        return PyInt_FromLong(following);
    }

    return PyErr_SetArgsError((PyObject *) self, "following", arg);
}

static PyObject *t_searchiterator_preceding(t_searchiterator *self,
                                            PyObject *arg)
{
    int32_t position, preceding;

    if (!parseArg(arg, "i", &position))
    {
        STATUS_CALL(preceding = self->object->preceding(position, status));
        return PyInt_FromLong(preceding);
    }

    return PyErr_SetArgsError((PyObject *) self, "preceding", arg);
}

static PyObject *t_searchiterator_reset(t_searchiterator *self)
{
    self->object->reset();
    Py_RETURN_NONE;
}

static PyObject *t_searchiterator_iter(t_searchiterator *self)
{
    Py_RETURN_SELF();
}

static PyObject *t_searchiterator_iter_next(t_searchiterator *self)
{
    int32_t offset;

    STATUS_CALL(offset = self->object->next(status));

    if (offset == USEARCH_DONE)
    {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    return PyInt_FromLong(offset);
}


/* StringSearch */

static int t_stringsearch_init(t_stringsearch *self,
                               PyObject *args, PyObject *kwds)
{
    UnicodeString *u0, _u0;
    UnicodeString *u1, _u1;
    Locale *locale;
    BreakIterator *iterator;
    RuleBasedCollator *collator;
    CharacterIterator *chars;

    switch (PyTuple_Size(args)) {
      case 3:
        if (!parseArgs(args, "SSP",
                       TYPE_CLASSID(Locale),
                       &u0, &_u0, &u1, &_u1, &locale))
        {
            INT_STATUS_CALL(self->object = new StringSearch(*u0, *u1, *locale,
                                                            NULL, status));
            self->flags = T_OWNED;
            self->collator = NULL;
            self->iterator = NULL;
            break;
        }
        if (!parseArgs(args, "SSP",
                       TYPE_CLASSID(RuleBasedCollator),
                       &u0, &_u0, &u1, &_u1, &collator))
        {
            INT_STATUS_CALL(self->object = new StringSearch(*u0, *u1, collator,
                                                            NULL, status));
            self->flags = T_OWNED;
            self->collator = PyTuple_GetItem(args, 2);
            Py_INCREF(self->collator);
            self->iterator = NULL;
            break;
        }
        if (!parseArgs(args, "SPP",
                       TYPE_ID(CharacterIterator),
                       TYPE_CLASSID(Locale),
                       &u0, &_u0, &chars, &locale))
        {
            INT_STATUS_CALL(self->object =
                            new StringSearch(*u0, *chars, *locale,
                                             NULL, status));
            self->flags = T_OWNED;
            self->collator = NULL;
            self->iterator = NULL;
            break;
        }
        if (!parseArgs(args, "SPP",
                       TYPE_ID(CharacterIterator),
                       TYPE_CLASSID(RuleBasedCollator),
                       &u0, &_u0, &chars, &collator))
        {
            INT_STATUS_CALL(self->object =
                            new StringSearch(*u0, *chars, collator,
                                             NULL, status));
            self->flags = T_OWNED;
            self->collator = PyTuple_GetItem(args, 2);
            Py_INCREF(self->collator);
            self->iterator = NULL;
            break;
        }
        PyErr_SetArgsError((PyObject *) self, "__init__", args);
        return -1;
      case 4:
        if (!parseArgs(args, "SSPP",
                       TYPE_CLASSID(Locale),
                       TYPE_ID(BreakIterator),
                       &u0, &_u0, &u1, &_u1, &locale, &iterator))
        {
            INT_STATUS_CALL(self->object = new StringSearch(*u0, *u1, *locale,
                                                            iterator, status));
            self->flags = T_OWNED;
            self->iterator = PyTuple_GetItem(args, 3);
            Py_INCREF(self->iterator);
            break;
        }
        if (!parseArgs(args, "SSPP",
                       TYPE_CLASSID(RuleBasedCollator),
                       TYPE_ID(BreakIterator),
                       &u0, &_u0, &u1, &_u1, &collator, &iterator))
        {
            INT_STATUS_CALL(self->object = new StringSearch(*u0, *u1, collator,
                                                            NULL, status));
            self->flags = T_OWNED;
            self->collator = PyTuple_GetItem(args, 2);
            Py_INCREF(self->collator);
            self->iterator = PyTuple_GetItem(args, 3);
            Py_INCREF(self->iterator);
            break;
        }
        if (!parseArgs(args, "SPPP",
                       TYPE_ID(CharacterIterator),
                       TYPE_CLASSID(Locale),
                       TYPE_ID(BreakIterator),
                       &u0, &_u0, &chars, &locale, &iterator))
        {
            INT_STATUS_CALL(self->object =
                            new StringSearch(*u0, *chars, *locale,
                                             iterator, status));
            self->flags = T_OWNED;
            self->collator = NULL;
            self->iterator = PyTuple_GetItem(args, 3);
            Py_INCREF(self->iterator);
            break;
        }
        if (!parseArgs(args, "SPPP",
                       TYPE_ID(CharacterIterator),
                       TYPE_CLASSID(RuleBasedCollator),
                       TYPE_ID(BreakIterator),
                       &u0, &_u0, &chars, &collator, &iterator))
        {
            INT_STATUS_CALL(self->object =
                            new StringSearch(*u0, *chars, collator,
                                             iterator, status));
            self->flags = T_OWNED;
            self->collator = PyTuple_GetItem(args, 2);
            Py_INCREF(self->collator);
            self->iterator = PyTuple_GetItem(args, 3);
            Py_INCREF(self->iterator);
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

static void t_stringsearch_dealloc(t_stringsearch *self)
{
    if (self->object)
    {
        if (self->flags & T_OWNED)
            delete self->object;
            
        self->object = NULL;

        Py_XDECREF(self->iterator); self->iterator = NULL;
        Py_XDECREF(self->collator); self->collator = NULL;
    }

    self->ob_type->tp_free((PyObject *) self);
}

static PyObject *t_stringsearch_getCollator(t_stringsearch *self)
{
    if (self->collator)
    {
        Py_INCREF(self->collator);
        return self->collator;
    }

    return wrap_RuleBasedCollator(self->object->getCollator(), 0);
}

static PyObject *t_stringsearch_setCollator(t_stringsearch *self,
                                            PyObject *arg)
{
    RuleBasedCollator *collator;

    if (!parseArg(arg, "P", TYPE_CLASSID(RuleBasedCollator), &collator))
    {
        Py_INCREF(arg); Py_XDECREF(self->collator); self->collator = arg;
        STATUS_CALL(self->object->setCollator(collator, status));
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setCollator", arg);
}

static PyObject *t_stringsearch_getPattern(t_stringsearch *self)
{
    UnicodeString u = self->object->getPattern();
    return PyUnicode_FromUnicodeString(&u);
}

static PyObject *t_stringsearch_setPattern(t_stringsearch *self, PyObject *arg)
{
    UnicodeString *u, _u;

    if (!parseArg(arg, "S", &u, &_u))
    {
        STATUS_CALL(self->object->setPattern(*u, status));
        Py_RETURN_NONE;
    }

    return PyErr_SetArgsError((PyObject *) self, "setPattern", arg);
}

static PyObject *t_stringsearch_str(t_stringsearch *self)
{
    UnicodeString u = self->object->getPattern();
    return PyUnicode_FromUnicodeString(&u);
}

DECLARE_RICHCMP(StringSearch, t_stringsearch);


void _init_search(PyObject *m)
{
    SearchIteratorType.tp_dealloc = (destructor) t_searchiterator_dealloc;
    SearchIteratorType.tp_iter = (getiterfunc) t_searchiterator_iter;
    SearchIteratorType.tp_iternext = (iternextfunc) t_searchiterator_iter_next;
    StringSearchType.tp_dealloc = (destructor) t_stringsearch_dealloc;
    StringSearchType.tp_str = (reprfunc) t_stringsearch_str;
    StringSearchType.tp_richcompare = (richcmpfunc) t_stringsearch_richcmp;

    INSTALL_CONSTANTS_TYPE(USearchAttribute, m);
    INSTALL_CONSTANTS_TYPE(USearchAttributeValue, m);

    INSTALL_TYPE(SearchIterator, m);
    REGISTER_TYPE(StringSearch, m);

    INSTALL_ENUM(USearchAttribute, USEARCH_OVERLAP);
    INSTALL_ENUM(USearchAttribute, USEARCH_CANONICAL_MATCH);
    INSTALL_ENUM(USearchAttribute, USEARCH_ELEMENT_COMPARISON);

    INSTALL_ENUM(USearchAttributeValue, USEARCH_DEFAULT);
    INSTALL_ENUM(USearchAttributeValue, USEARCH_OFF);
    INSTALL_ENUM(USearchAttributeValue, USEARCH_ON);
    INSTALL_ENUM(USearchAttributeValue, USEARCH_STANDARD_ELEMENT_COMPARISON);
    INSTALL_ENUM(USearchAttributeValue, USEARCH_PATTERN_BASE_WEIGHT_IS_WILDCARD);
    INSTALL_ENUM(USearchAttributeValue, USEARCH_ANY_BASE_WEIGHT_IS_WILDCARD);
}
