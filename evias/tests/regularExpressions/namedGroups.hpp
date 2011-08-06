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
                if (! (bool) _returnCode)
                    return _returnCode;

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

                if (r.lastReturnCode() != PARSE_DONE) {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                evias::core::containers::nmatches n = r.getNamedMatches();

                // test named matches
                if (n["__auto__entire_match"] != "eVs92188") {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }
                else if (n["user_id"] != "eVs") {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }
                else if (n["user_code"] != "92188") {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                string t2[3] = {
                    "user_id",
                    "login",
                    "acl_role"
                };
                vector<string> g2(t2, t2+3);

                r.setPattern(
                    "([1-9][0-9]+);" // user_id
                    "([A-Za-z][A-Za-z0-9_\\-\\.]+);" // login
                    "(admin|guest);" // acl_role
                );
                r.setGroups(g2);

                r.parse("329081988;evias92;admin;");

                if (r.lastReturnCode() != PARSE_DONE) {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                n = r.getNamedMatches();

                // test named matches
                if (n["__auto__entire_match"] != "329081988;evias92;admin;") {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }
                else if (n["user_id"] != "329081988") {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }
                else if (n["login"] != "evias92") {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }
                else if (n["acl_role"] != "admin") {
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

