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

                assertableString<const char*>::assertEqual(m[0].c_str(), "EVS4565");
                assertableString<const char*>::assertEqual(m[1].c_str(), "EVS");
                assertableString<const char*>::assertEqual(m[2].c_str(), "4565");
                
                r.setPattern("([A-Za-z0-9]{4})([4-8]{2})([GYCA]{1})");
                r.parse("EVS188G");

                m = r.getIndexedMatches();
                
                assertableString<const char*>::assertEqual(m[0].c_str(), "EVS188G");
                assertableString<const char*>::assertEqual(m[1].c_str(), "EVS1");
                assertableString<const char*>::assertEqual(m[2].c_str(), "88");
                assertableString<const char*>::assertEqual(m[3].c_str(), "G");

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

