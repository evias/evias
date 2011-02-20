#ifndef __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_CHARSRANGES__
#define __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_CHARSRANGES__

/**
 * Package : eVias unitary test suite
 *
 * Copyright (c) 2010 - 2011 Grégory Saive
 *
 * For more informations about the licensing of this product, please refer
 * to the LICENCE file in the root application directory.
 *
 * Version: 	1.0
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "core/regexp_chars_group.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core  {

namespace test {

namespace regularExpressions {

    using evias::core::regexp::charsGroup;
    using evias::core::assemble;
    using evias::core::intToString;

    class charsRanges : public unitTest
    {
        public :

            charsRanges () : unitTest () { };

            ~charsRanges () {};

            inline string wrongMatchingCharsError(string object, string expected)
            {
                return ("Wrong matching chars after group parse. [Expected: \""
                    + expected
                    + "\" Got: \""
                    + object + "\""
                );
            }

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                if (! (bool) _returnCode)
                    return _returnCode;

                // easy parse, try and see if the matching chars are well sorted
                string groupPattern = "abc043X@";
                charsGroup testGroup;
                testGroup.setPattern(groupPattern);

                string objectMatchingChars   = evias::core::assemble(testGroup.getMatchingChars(), "");
                string expectedMatchingChars = "034Xabc@";
                if (objectMatchingChars != expectedMatchingChars) {
                    _returnMsg = wrongMatchingCharsError(objectMatchingChars, expectedMatchingChars);
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // easy ranges with little sort test
                groupPattern = "A-Z4-73";
                testGroup.setPattern(groupPattern);

                objectMatchingChars = evias::core::assemble(testGroup.getMatchingChars(), "");
                expectedMatchingChars = "34567ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                if (objectMatchingChars != expectedMatchingChars) {
                    _returnMsg = wrongMatchingCharsError(objectMatchingChars, expectedMatchingChars);
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // mid strong ranges, what a dirty mind could produce this one ?!
                groupPattern = "A-E023-6f-lF-Za-em-z7-91";
                testGroup.setPattern(groupPattern);

                objectMatchingChars = evias::core::assemble(testGroup.getMatchingChars(), "");
                expectedMatchingChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
                if (objectMatchingChars != expectedMatchingChars) {
                    _returnMsg = wrongMatchingCharsError(objectMatchingChars, expectedMatchingChars);
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // excluding rule, as this chars group will match:
                // ABCDEFG0123456789
                // we should match all BUT these characters.
                groupPattern = "^A-G012345-9";
                testGroup.setPattern(groupPattern);

                objectMatchingChars = evias::core::assemble(testGroup.getMatchingChars(), "");

                // the expectedMatchingChars can be gotten by parsing the pattern
                // without the first character and then do the exclude by ourself.
                testGroup.setPattern(groupPattern.substr(1, groupPattern.size()-1));

                // do the exclude
                vector<string> anyDictionnary = evias::core::splitParts(evias::core::__mAnyDictionnary, 1);
                anyDictionnary = evias::core::vector_remove(anyDictionnary, testGroup.getMatchingChars());
                expectedMatchingChars = evias::core::assemble(anyDictionnary, "");

                if (objectMatchingChars != expectedMatchingChars) {
                    _returnMsg = wrongMatchingCharsError(objectMatchingChars, expectedMatchingChars);
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                return setReturnCode((int) RETURN_SUCCESS);
            }

            inline int shutdown ()
            {
                return _returnCode;
            }

    };

}; // end namespace regularExpressions

}; // end namespace test

}; // end namespace core

}; // end namespace evias

#endif

