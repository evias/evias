#ifndef __EVIAS_CORE_TEST_FUNCTIONAL_NS_FUNCTORTEST__
#define __EVIAS_CORE_TEST_FUNCTIONAL_NS_FUNCTORTEST__

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

#include "core/functional.hpp"
#include "core/common_utils.hpp"

namespace evias {

namespace core  {

namespace test {

namespace functional {

    using std::vector;
    using std::string;
    using evias::core::intToString;

    /**
     * @brief
     * a bunch of callback functions that are used in
     * this unitary test to test the evias::core::functor
     * features which allows executing functions by setting
     * a "callback" where callback refers to the function pointer
     * address.
     **/
    inline bool cb_stringEquals(const char* s1, const char* s2)
        { return strcmp(s1, s2) == 0; }

    inline bool cb_stringBigger(const char* s1, const char* s2)
        { return strcmp(s1, s2) == -1; }

    inline bool cb_stringSmaller(const char* s1, const char* s2)
        { return strcmp(s1, s2) == 1; }

    inline int cb_increaseStaticCounterAndAddition(int i1, int i2)
    {
        static int counter = 0;

        return (++counter) + (i1 + i2);
    }

    class functorTest : public unitTest
    {
        public :

            functorTest () : unitTest () { };

            ~functorTest () {};

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                assertable<bool>::assertEqual((bool) _returnCode, true);

                // define the callback function type
                // for the three string comparing methods the signature
                // remains pretty simple as we return a bool and take 2 string
                typedef bool (*__function) (const char*,const char*);

                // define callbacks
                evias::core::callback<bool, const char*, __function>* cbEquals  = new callback<bool, const char*, __function>(&cb_stringEquals);
                evias::core::callback<bool, const char*, __function>* cbBigger  = new callback<bool, const char*, __function>(&cb_stringBigger);
                evias::core::callback<bool, const char*, __function>* cbSmaller = new callback<bool, const char*, __function>(&cb_stringSmaller);

                // we are going to execute each function 2 times
                // - cb_stringEquals once TRUE, once FALSE
                // - cb_stringBigger once TRUE, once FALSE
                // - cb_stringSmaller once TRUE, once FALSE

                assertable<bool>::assertEqual(cbEquals->execute("hello", "hello"), true);
                assertable<bool>::assertEqual(cbEquals->execute("hello", "hellO"), false);
                
                assertable<bool>::assertEqual(cbBigger->execute("hEllo", "hello"), true);
                assertable<bool>::assertEqual(cbBigger->execute("hello", "hEllo"), false);
                
                assertable<bool>::assertEqual(cbSmaller->execute("hello", "hEllo"), true);
                assertable<bool>::assertEqual(cbSmaller->execute("hEllo", "hello"), false);

                // define new callback function type
                typedef int (*__function2) (int,int);

                // initialize functor
                evias::core::callback<int, int, __function2>* cbStaticCnt = new callback<int, int, __function2>(&cb_increaseStaticCounterAndAddition);

                // see if the static counter increases correctly (+1 always)
                assertable<int>::assertEqual(cbStaticCnt->execute(2,5), 8);
                assertable<int>::assertEqual(cbStaticCnt->execute(3,6), 11);
                assertable<int>::assertEqual(cbStaticCnt->execute(1,1), 5);

                // final test, the counter should have been increased 3 times (3 calls)
                // and will be increased before the addition of 0+0, so we got to need 4 ;)
                assertable<int>::assertEqual(cbStaticCnt->execute(0,0), 4);

                delete cbEquals;
                delete cbBigger;
                delete cbSmaller;
                delete cbStaticCnt;

                return setReturnCode((int) RETURN_SUCCESS);
            }

            inline int shutdown ()
            {
                return _returnCode;
            }

    };

}; // end namespace functional

}; // end namespace test

}; // end namespace core

}; // end namespace evias

#endif

