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

%module PyICU_iterators

%{

#include "common.h"

%}

%include "common.i"
%import "bases.i"

namespace icu {

    class ForwardCharacterIterator : public UObject {
    public:
        enum {
            DONE = 0xffff
        };

        UBool operator==(ForwardCharacterIterator &);
        UBool operator!=(ForwardCharacterIterator &);

        int32_t hashCode();
        UChar nextPostInc();
        UBool hasNext();
    };

    class CharacterIterator : public ForwardCharacterIterator {
    public:
        enum EOrigin {
            kStart,
            kCurrent,
            kEnd
        };

        CharacterIterator *clone();
        UChar first();
        UChar firstPostInc();
        UChar last();
        UChar current();
        UChar next();
        UChar previous();

        int32_t setToStart();
        int32_t setToEnd();

        UChar setIndex(int32_t);
        UBool hasPrevious();

        int32_t startIndex();
        int32_t endIndex();
        int32_t getIndex();
        int32_t getLength();
        int32_t move(int32_t, EOrigin);
        void getText(UnicodeString &);
    };

    class UCharCharacterIterator : public CharacterIterator {
    public:
        UCharCharacterIterator(UChar *, _int32_t);
        UCharCharacterIterator(UChar *, _int32_t, int32_t);
        UCharCharacterIterator(UChar *, _int32_t, int32_t, int32_t, int32_t);
    };

    class StringCharacterIterator : public CharacterIterator {
    public:

        StringCharacterIterator(UnicodeString &);
        StringCharacterIterator(_PyString);
        StringCharacterIterator(UnicodeString &, int32_t);
        StringCharacterIterator(_PyString, int32_t);
        StringCharacterIterator(UnicodeString &, int32_t, int32_t, int32_t);
        StringCharacterIterator(_PyString, int32_t, int32_t, int32_t);

        void setText(UnicodeString &);
        void setText(_PyString);
    };

    class BreakIterator : public UObject {
    public:
        
        UBool operator==(BreakIterator &);
        UBool operator!=(BreakIterator &);
        BreakIterator *clone();

	void adoptText(CharacterIterator *);

	void setText(UnicodeString &);
	void setText(_PyString);

	int32_t first();
	int32_t last();
	int32_t previous();
	int32_t next();
	int32_t next(int32_t);
	int32_t current();
	int32_t following(int32_t);
	int32_t preceding(int32_t);

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
        }

        static BreakIterator *createWordInstance(Locale &, UErrorCode);
        static BreakIterator *createLineInstance(Locale &, UErrorCode);
        static BreakIterator *createCharacterInstance(Locale &, UErrorCode);
        static BreakIterator *createSentenceInstance(Locale &, UErrorCode);
        static BreakIterator *createTitleInstance(Locale &, UErrorCode);
        static LocaleArray1 getAvailableLocales(_int32_t);
        static UnicodeString2 &getDisplayName(Locale &, Locale &, UnicodeString &);
        static UnicodeString1 &getDisplayName(Locale &, UnicodeString &);
        static UnicodeString getDisplayName(Locale &, Locale &, _UnicodeString);
        static UnicodeString getDisplayName(Locale &, _UnicodeString);
    };

    class RuleBasedBreakIterator : public BreakIterator {
    public:
        RuleBasedBreakIterator();
        RuleBasedBreakIterator(UnicodeString &, UParseError, UErrorCode);
        RuleBasedBreakIterator(_PyString, UParseError, UErrorCode);
        
        UnicodeString getRules();
        UBool isBoundary(int32_t);
        int32_t getRuleStatus();
    };

    class DictionaryBasedBreakIterator : public RuleBasedBreakIterator {
    public:
        DictionaryBasedBreakIterator();
    };

    class CanonicalIterator : public UObject {
    public:
        CanonicalIterator(UnicodeString &, UErrorCode);
        CanonicalIterator(_PyString, UErrorCode);

        UnicodeString getSource();
        void setSource(UnicodeString &, UErrorCode);
        void setSource(_PyString, UErrorCode);
        void reset();
        UnicodeString next();
    };

    class CollationElementIterator : public UObject {
    public:
        UBool operator==(CollationElementIterator &);
        UBool operator!=(CollationElementIterator &);

        void reset();
        int32_t next(UErrorCode);
        int32_t previous(UErrorCode);
        int32_t getMaxExpansion(int32_t);
        int32_t strengthOrder(int32_t);
        
        void setText(UnicodeString &, UErrorCode);
        void setText(_PyString, UErrorCode);
        void setText(CharacterIterator &, UErrorCode);

        int32_t getOffset();
        void setOffset(int32_t, UErrorCode);

        static int32_t primaryOrder(int32_t);
        static int32_t secondaryOrder(int32_t);
        static int32_t tertiaryOrder(int32_t);
        static UBool isIgnorable(int32_t);
    };
}
