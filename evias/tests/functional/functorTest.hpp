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

                // initialize functor class saying we return bool,
                // we want to execute a __function (see typedef above),
                // and we are passing string values.
                evias::core::functor<bool, __function, const char*> *callbackHandler = new evias::core::functor<bool, __function, const char*>();

                // we are going to execute each function 2 times
                // - cb_stringEquals once TRUE, once FALSE
                // - cb_stringBigger once TRUE, once FALSE
                // - cb_stringSmaller once TRUE, once FALSE
                __function callback = NULL;

                // cb_stringEquals
                callback = &cb_stringEquals;
                callbackHandler->setCallback(callback);
                assertable<bool>::assertEqual(callbackHandler->execute("hello", "hello"), true);
                assertable<bool>::assertEqual(callbackHandler->execute("hello", "hellO"), false);

                callback = NULL;

                // cb_stringBigger
                callback = &cb_stringBigger;
                callbackHandler->setCallback(callback);
                assertable<bool>::assertEqual(callbackHandler->execute("hEllo", "hello"), true);
                assertable<bool>::assertEqual(callbackHandler->execute("hello", "hEllo"), false);
                
                callback = NULL;

                // cb_stringSmaller
                callback = &cb_stringSmaller;
                callbackHandler->setCallback(callback);
                assertable<bool>::assertEqual(callbackHandler->execute("hello", "hEllo"), true);
                assertable<bool>::assertEqual(callbackHandler->execute("hEllo", "hello"), false);

                delete callbackHandler;

                // define new callback function type
                typedef int (*__function2) (int,int);

                // initialize functor
                evias::core::functor<int, __function2, int> *handler2 = new evias::core::functor<int, __function2, int>();
                handler2->setCallback(&cb_increaseStaticCounterAndAddition);

                // see if the static counter increases correctly (+1 always)
                assertable<int>::assertEqual(handler2->execute(2,5), 8);
                assertable<int>::assertEqual(handler2->execute(3,6), 11);
                assertable<int>::assertEqual(handler2->execute(1,1), 5);

                // final test, the counter should have been increased 3 times (3 calls)
                // and will be increased before the addition of 0+0, so we got to need 4 ;)
                assertable<int>::assertEqual(handler2->execute(0,0), 4);

                delete handler2;

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

