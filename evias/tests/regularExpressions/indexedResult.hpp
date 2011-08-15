#ifndef __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_INDEXEDRESULT__
#define __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_INDEXEDRESULT__

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

namespace evias {

namespace core  {

namespace test {

namespace regularExpressions {

    using evias::core::assemble;
    using evias::core::intToString;

    class indexedResult : public unitTest
    {
        public :

            indexedResult () : unitTest () { };

            ~indexedResult () {};

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                if (! (bool) _returnCode)
                    return _returnCode;

                // test parse reliability
                evias::core::regex r("^EVS([1-9]{2})+$");

                assertable<int>::assertEqual(r.parse("EVS00"), (int) PARSE_FAILED);
                assertable<int>::assertEqual(r.parse("EVS124567"), (int) PARSE_DONE);
                
                r.setPattern("^(EVS)([1-9]{4})");
                r.parse("EVS4565");

                evias::core::containers::imatches m = r.getIndexedMatches();

                assertable<string>::assertEqual(m[0], "EVS4565");
                assertable<string>::assertEqual(m[1], "EVS");
                assertable<string>::assertEqual(m[2], "4565");
                
                r.setPattern("([A-Za-z0-9]{4})([4-8]{2})([GYCA]{1})");
                r.parse("EVS188G");

                m = r.getIndexedMatches();
                
                assertable<string>::assertEqual(m[0], "EVS188G");
                assertable<string>::assertEqual(m[1], "EVS1");
                assertable<string>::assertEqual(m[2], "88");
                assertable<string>::assertEqual(m[3], "G");

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

