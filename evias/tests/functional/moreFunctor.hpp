#ifndef __EVIAS_CORE_TEST_FUNCTIONAL_NS_MOREFUNCTOR__
#define __EVIAS_CORE_TEST_FUNCTIONAL_NS_MOREFUNCTOR__

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

    struct my_return_type_t
    {
        bool isEqualValues;
        int  counter;
    };

    struct my_user_type_t
    {
        int id;
        string login;
        string pass;
    };

    /**
     * @brief
     * a bunch of callback functions that are used in
     * this unitary test to test the evias::core::functor
     * features which allows executing functions by setting
     * a "callback" where callback refers to the function pointer
     * address.
     **/
    inline my_return_type_t cb_increaseCounterAndReturnBrief(int i1, int i2)
    {
        my_return_type_t r;

        static int counter = 0;

        r.isEqualValues = i1 == i2;
        r.counter = (++counter);

        return r;
    }

    inline my_user_type_t cb_createUserIncreasesID(string login, string pass)
    {
        static int counter = 0;
        
        my_user_type_t user;
        user.id = (++counter);
        user.login = login;
        user.pass  = pass;

        return user;
    }

    inline string cb_getMergedUsersToCSV(my_user_type_t u1, my_user_type_t u2)
    {
        my_user_type_t merged;

        merged.id = u1.id + u2.id;
        merged.login = u1.login + "-" + u2.login;
        merged.pass  = u1.pass + "-" + u2.pass;

        return intToString(merged.id) + ";" + merged.login + ";" + merged.pass + ";";
    }

    inline bool cb_stringEquals(string s1, string s2)
    {
        return strcmp (s1.c_str(), s2.c_str()) == 0;
    }

    class moreFunctor : public unitTest
    {
        public :

            moreFunctor () : unitTest () { };

            ~moreFunctor () {};

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                assertable<bool>::assertEqual((bool) _returnCode, true);

                // define callback function type and functor type
                typedef my_return_type_t (*callback1_t) (int, int);
                typedef evias::core::callback<my_return_type_t, int, callback1_t> callback1_class_t;

                callback1_class_t* cbIncreaseCnt = new callback1_class_t(&cb_increaseCounterAndReturnBrief);

                my_return_type_t test;

                // first execution > counter=1, (1==1)=true
                test = cbIncreaseCnt->execute(1, 1);
                assertable<bool>::assertEqual(test.isEqualValues, true);
                assertable<int>::assertEqual(test.counter, 1);

                // second execution > counter=2, (3==4)=false
                test = cbIncreaseCnt->execute(3, 4);
                assertable<bool>::assertEqual(test.isEqualValues, false);
                assertable<int>::assertEqual(test.counter, 2);

                // third execution > counter=3, (-1==-1)=true
                test = cbIncreaseCnt->execute(-1, -1);
                assertable<bool>::assertEqual(test.isEqualValues, true);
                assertable<int>::assertEqual(test.counter, 3);

                delete cbIncreaseCnt;

                // define next callback function type and functor type
                typedef my_user_type_t (*callback2_t) (string, string);
                typedef evias::core::callback<my_user_type_t, string, callback2_t> callback2_class_t;

                callback2_class_t* cbCreateUser = new callback2_class_t(&cb_createUserIncreasesID);

                // try execution with custom return type.
                my_user_type_t testUser = cbCreateUser->execute("greg", "my_password");
                assertable<int>::assertEqual(testUser.id, 1);
                assertable<string>::assertEqual(testUser.login, "greg");
                assertable<string>::assertEqual(testUser.pass, "my_password");

                delete cbCreateUser;

                // define one more callback / functor types pair and
                // one assertion callback type
                typedef string (*callback3_t) (my_user_type_t, my_user_type_t);
                typedef evias::core::callback<string, my_user_type_t, callback3_t> callback3_class_t;

                callback3_class_t* cbMerge = new callback3_class_t(&cb_getMergedUsersToCSV);

                // test data
                my_user_type_t u1,u2;
                u1.id = 1;
                u1.login = "greg";
                u1.pass  = "debut";
                u2.id = 2;
                u2.login = "ory";
                u2.pass  = "fin";

                // use callback handler to execute the merge function
                // the result will then be tested using an assertion
                string resultCSV = cbMerge->execute(u1, u2);

                // configure assertion callback method (operator to use)
                // (@see bool cb_stringEquals(string,string) defined above)
                typedef bool (*assert_callback_t) (string, string);
                assert_callback_t cb4 = &cb_stringEquals;

                // use execution of cb_stringEqual with (resultCSV, "...") to determine
                // the assertion trueness. (if this works, defining custom operators
                // for assertions is then possible)
                assertable<string>::assertEqual(resultCSV, "3;greg-ory;debut-fin;", cb4);

                delete cbMerge;

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

