#ifndef __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_OCCURENCECOUNTERS__
#define __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_OCCURENCECOUNTERS__

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

    class occurenceCounters : public unitTest
    {
        public :

            occurenceCounters () : unitTest () { };

            ~occurenceCounters () {};

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
                string regexp = "[abcdef]{4}";
                eRegExp testObject(regexp, "no_value_test");
                if (testObject.getGroups().size() != 4) {
                    _returnMsg = "\"" + regexp + "\" should match 4 positions, though matches " + intToString(testObject.getGroups().size()) + ".";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // 5 [abcd] occurences followed by 2 [0-5]
                // test the groups count
                // test matching chars for position one before the end.
                regexp = "[abcd]{5}[0-5]{2}";
                testObject.parse(regexp, "no_value_test");
                int objectGroupsCnt = testObject.getGroups().size();
                int expectedGroupsCnt = 7;
                if (objectGroupsCnt != expectedGroupsCnt) {
                    _returnMsg = "\"" + regexp + "\" should match " + intToString(expectedGroupsCnt) + ", though matches " + intToString(objectGroupsCnt) + ".";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }
                map<int, charsGroup> objectGroups = testObject.getGroups();
                string objectMatching   = evias::core::assemble(objectGroups[5].getMatchingChars(), "");
                string expectedMatching = "012345";
                if (objectMatching != expectedMatching) {
                    _returnMsg = wrongMatchingCharsError(objectMatching, expectedMatching);
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // test multiple occurence counters, test one with escaped character
                regexp = "evias[0-9]{2}[\\-_]{1}[1-5]{2}";
                testObject.parse(regexp, "no_value_test");
                objectGroupsCnt   = testObject.getGroups().size();
                expectedGroupsCnt = 10;
                // test positions count
                if (objectGroupsCnt != expectedGroupsCnt) {
                    _returnMsg = "\""+regexp+"\" should match "+intToString(expectedGroupsCnt)+", though matches "+intToString(objectGroupsCnt)+".";
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // test matching characters for [\-_] pattern
                objectGroups = testObject.getGroups();
                objectMatching   = evias::core::assemble(objectGroups[7].getMatchingChars(), "");
                expectedMatching = "-_";
                if (objectMatching != expectedMatching) {
                    _returnMsg = wrongMatchingCharsError(objectMatching, expectedMatching);
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }
                // test matching characters for [0-5]
                objectMatching   = evias::core::assemble(objectGroups[8].getMatchingChars(), "");
                expectedMatching = "12345";
                if (objectMatching != expectedMatching) {
                    _returnMsg = wrongMatchingCharsError(objectMatching, expectedMatching);
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }
                // test matching characters for [0-9]
                objectMatching   = evias::core::assemble(objectGroups[5].getMatchingChars(), "");
                expectedMatching = "0123456789";
                if (objectMatching != expectedMatching) {
                    _returnMsg = wrongMatchingCharsError(objectMatching, expectedMatching);
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

