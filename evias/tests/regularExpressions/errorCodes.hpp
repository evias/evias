#ifndef __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_ERRORCODES__
#define __EVIAS_CORE_TEST_REGULAREXPRESSIONS_NS_ERRORCODES__

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

    using evias::core::intToString;

    class errorCodes : public unitTest
    {
        public :

            errorCodes () : unitTest () { };

            ~errorCodes () {};

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                if (! (bool) _returnCode)
                    return _returnCode;

                // test DATA_MISS error code
                evias::core::regex empty("ABC");
                empty.parse();
                if (empty.lastReturnCode() != DATA_MISS) {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // test SYNTAX_ERROR error code
                evias::core::regex errExpr("(ABC-");
                if (errExpr.lastReturnCode() != SYNTAX_ERROR) {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // test SYNTAX_OK error code
                evias::core::regex easy("[A-Z]{2}[0-9]+");
                if (easy.lastReturnCode() != SYNTAX_OK) {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }
                
                // test PARSE_DONE error code
                easy.parse("AB003");
                if (easy.lastReturnCode() != PARSE_DONE) {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // test PARSE_FAILED error code
                easy.parse("12AZ");
                if (easy.lastReturnCode() != PARSE_FAILED) {
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

