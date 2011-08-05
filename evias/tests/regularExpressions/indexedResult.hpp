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
                if (r.parse("EVS00") != PARSE_FAILED) {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }
                if (r.parse("EVS124567") != PARSE_DONE) {
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

