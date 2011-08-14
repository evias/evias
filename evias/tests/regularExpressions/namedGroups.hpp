#ifndef __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_NAMEDGROUPS__
#define __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_NAMEDGROUPS__

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

    class namedGroups : public unitTest
    {
        public :

            namedGroups () : unitTest () { };

            ~namedGroups () {};

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                assertable<bool>::assertEqual((bool) _returnCode, true);

                string t[2] = {"user_id","user_code"};
                vector<string> g(t, t+2);
                evias::core::regex r("([A-Za-z]{3})([1-9]{5})");

                // configure the named groups for the current pattern
                // 2 groups .. first one is user_id, second one is user_code
                r.setGroups(g);

                // we want to get a map<string,string> with 3 elements
                // 1st being the whole match ; 2nd being "eVs", user_id
                // 3rd being "92188", user_code
                r.parse("eVs92188");

                assertable<int>::assertEqual((int) r.lastReturnCode(), (int) PARSE_DONE);

                evias::core::containers::imatches m = r.getIndexedMatches();
                evias::core::containers::nmatches n = r.getNamedMatches();

                // test indexed matches
                assertableString<const char*>::assertEqual(m[0].c_str(), "eVs92188");
                assertableString<const char*>::assertEqual(m[1].c_str(), "eVs");
                assertableString<const char*>::assertEqual(m[2].c_str(), "92188");

                // test named matches
                assertableString<const char*>::assertEqual(n["__auto__entire_match"].c_str(), "eVs92188");
                assertableString<const char*>::assertEqual(n["user_id"].c_str(), "eVs");
                assertableString<const char*>::assertEqual(n["user_code"].c_str(), "92188");

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

