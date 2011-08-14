#ifndef __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_BOOSTREGEX__
#define __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_BOOSTREGEX__

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

#include <boost/regex.hpp>
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core  {

namespace test {

namespace regularExpressions {

    using evias::core::intToString;

    class boostRegex : public unitTest
    {
        public :

            boostRegex () : unitTest () { };

            ~boostRegex () {};

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                if (! (bool) _returnCode)
                    return _returnCode;

                // test easy match
                boost::regex expr("^EVS([A-Za-z0-9_]{3})+$");
                boost::cmatch what;
                
                assertable<bool>::assertEqual(boost::regex_match("EVS0056_3", what, expr), true);

                // test match with subexpressions
                expr.assign(boost::regex("^(?<group_name>[ABC]{2})(?<user_id>[0-9]+)$"));
                boost::cmatch matches;

                assertable<bool>::assertEqual(boost::regex_match("AB092100", matches, expr), true);

                // iterate matches and test values
                for (int i = 0; i < matches.size(); i++) {
                    string match(matches[i].first, matches[i].second);

                    switch (i) {
                        case 0:
                            assertableString<const char*>::assertEqual(match.c_str(), "AB092100");
                            break;
                        case 1:
                            assertableString<const char*>::assertEqual(match.c_str(), "AB");
                            break;
                        case 2:
                            assertableString<const char*>::assertEqual(match.c_str(), "092100");
                            break;
                        default :
                            return setReturnCode((int) ERROR_EXTERNAL_UPDATE);
                    }
                }

                boost::smatch m;

                assertable<bool>::assertEqual(boost::regex_match(string("CC092100"), m, expr), true);

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

