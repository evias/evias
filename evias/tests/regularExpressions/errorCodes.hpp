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
                assertable<int>::assertEqual((int) empty.lastReturnCode(), (int) DATA_MISS);

                // test SYNTAX_ERROR error code
                try {
                    evias::core::regex errExpr("(ABC-");
                    assertable<int>::assertEqual((int) errExpr.lastReturnCode(), (int) SYNTAX_ERROR);
                }
                catch (const evias::core::syntaxError &e) {
                    // OK
                }
                catch (...) {
                    return setReturnCode((int) ERROR_DEVELOPMENT);
                }

                // test SYNTAX_OK error code
                evias::core::regex easy("[A-Z]{2}[0-9]+");
                assertable<int>::assertEqual((int) easy.lastReturnCode(), (int) SYNTAX_OK);

                // test PARSE_DONE error code
                easy.parse("AB003");
                assertable<int>::assertEqual((int) easy.lastReturnCode(), (int) PARSE_DONE);

                // test PARSE_FAILED error code
                easy.parse("12AZ");
                assertable<int>::assertEqual((int) easy.lastReturnCode(), (int) PARSE_FAILED);

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

