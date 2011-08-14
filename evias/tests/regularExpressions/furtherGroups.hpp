#ifndef __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_FURTHERGROUPS__
#define __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_FURTHERGROUPS__

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
#include "core/common_utils.hpp"

namespace evias {

namespace core  {

namespace test {

namespace regularExpressions {

    using std::vector;
    using std::string;
    using evias::core::intToString;

    class furtherGroups : public unitTest
    {
        public :

            furtherGroups () : unitTest () { };

            ~furtherGroups () {};

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                assertable<bool>::assertEqual((bool) _returnCode, true);

                string t[3] = {"subdomain", "domain", "network"};
                vector<string> g(t, t+3);
                evias::core::regex r("([a-zA-Z0-9]*)\\.([a-zA-Z0-9\\-]*)\\.([a-zA-Z]{2,3})");

                r.setGroups(g);
                r.parse("www.yourass.com");

                assertable<int>::assertEqual((int) r.lastReturnCode(), (int) PARSE_DONE);

                evias::core::containers::nmatches n = r.getNamedMatches();

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

