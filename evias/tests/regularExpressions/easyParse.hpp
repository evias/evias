#ifndef __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_EASYPARSE__
#define __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_EASYPARSE__

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

#include "core/regexp.hpp"
#include "core/regexp_chars_group.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core  {

namespace test {

namespace regularExpressions {

    using evias::core::regexp::eRegExp;

    using evias::core::intToString;

    class easyParse : public unitTest
    {
        public :

            easyParse () : unitTest () { };

            ~easyParse () {};

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

                // that breaks, i stop coding
                string regexp = "abc[def]123";
                eRegExp testObject(regexp, "no_value_test");
                if (testObject.getGroups().size() != 7) {
                    _returnMsg = "pattern \"" + regexp + "\" should match 7 positions, but matches: " + intToString(testObject.getGroups().size());
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // try and iterate through groups to see if we, correctly,
                // have one matching char for each group except for the 4th one,
                // where we should match 3 chars.
                map<int,charsGroup> groups = testObject.getGroups();
                int i = 0;
                for (map<int,charsGroup>::iterator it = groups.begin(); it != groups.end(); it++, i++) {
                    int cntMatch = (*it).second.getMatchingChars().size();
                    if (i == 3 && cntMatch != 3) {
                        _returnMsg = "third position of pattern " + regexp + " should match 3 chars, though matches " + intToString(cntMatch);
                        return setReturnCode((int) ERROR_DEVELOPMENT);
                    }
                    else if (i != 3 && cntMatch != 1) {
                        _returnMsg = "wrong matching char count for index " + intToString(i) + ".";
                        return setReturnCode((int) ERROR_DEVELOPMENT);
                    }
                }

                // get useful (french speaking belgian zipcodes :P)
                regexp = "B-[1-9][0-9][0-9][0-9]";
                testObject.parse(regexp, "no_value_test");
                map<int,charsGroup> zipCodeParts = testObject.getGroups();

                if (zipCodeParts.size() != 6) {
                    _returnMsg = "\"" + regexp + "\" should match 6 positions, though matches " + intToString(zipCodeParts.size()) + ".";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                vector<string> objectMatching = zipCodeParts[2].getMatchingChars();
                int cntExpecting = 9;
                if (objectMatching.size() != cntExpecting) {
                    _returnMsg = "\"" + regexp + "\"' second position should match 9 chars, though matches " + intToString(objectMatching.size()) + ".";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // use infinity operator
                regexp = "B-[1-9][0-9]+";
                testObject.parse(regexp, "no_value_test");
                zipCodeParts = testObject.getGroups();

                // should be matching at least 5 characters, as we can't define
                // a maximal size, we set it to be the minimal.
                if (zipCodeParts.size() != 4) {
                    _returnMsg = "\"" + regexp + "\" should match at least 4 positions, though matches " + intToString(zipCodeParts.size()) + ".";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // use infinity operator with following group
                regexp = "B-[1-9][0-9]+[abc]x[def]";
                testObject.parse(regexp, "no_value_test");
                zipCodeParts = testObject.getGroups();

                // matches minimum of 7 characters since the last groups have
                // no occurence counters and so must be present after any
                // count of occurences matching a char of the group before
                // of which the declaration is followed by a '+' operator.
                if (zipCodeParts.size() != 7) {
                    _returnMsg = "\"" + regexp + "\" should match at least 7 positions, though matches " + intToString(zipCodeParts.size()) + ".";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                int objectMatchingCnt = zipCodeParts[4].getMatchingChars().size();
                int expectedMatchingCnt = 3; // 5th position is [abc] => three matching chars
                if (objectMatchingCnt != expectedMatchingCnt) {
                    _returnMsg = "\"" + regexp + "\" should match " + intToString(expectedMatchingCnt) +" for pos 5, matches " + intToString(objectMatchingCnt) + ".";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                regexp = "B-[1-9][0-9]+[abc]?x[def]";
                testObject.parse(regexp, "no_value_test");
                zipCodeParts = testObject.getGroups();

                // '?' operator will set position to be matching the group concerned
                // but on value parse mode, it will enable the emptiness possibility
                // for this position (emptiness means group not being matched)
                if (zipCodeParts.size() != 7) {
                    _returnMsg = "\"" + regexp + "\" should match at least 7 positions, though matches " + intToString(zipCodeParts.size()) + ".";
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

