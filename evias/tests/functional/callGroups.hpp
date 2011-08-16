#ifndef __EVIAS_CORE_TEST_FUNCTIONAL_NS_CALLGROUPS__
#define __EVIAS_CORE_TEST_FUNCTIONAL_NS_CALLGROUPS__

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
    // first evias::core::callgroup will execute :
    // bool (*__my_decl) (string,string)
    inline bool cg1_stringEquals(string s1, string s2)
    {
        return strcmp (s1.c_str(), s2.c_str()) == 0;
    }
    inline bool cg1_stringBigger(string s1, string s2)
    {
        return strcmp (s1.c_str(), s2.c_str()) == -1;
    }
    inline bool cg1_stringSmaller(string s1, string s2)
    {
        return strcmp (s1.c_str(), s2.c_str()) == 1;
    }

    // second evias::core::callgroup will execute :
    // string (*__my_decl) (const char*, const char*)
    inline string cg2_getDisplayName(const char* fname, const char* lname)
    {
        return string (fname) + " " + string(lname);
    }
    inline string cg2_getCombinedName(const char* fname, const char* lname)
    {
        static int counter = 0;

        string combined = string(fname) + string(lname);
        if (combined.find("Saive") == string::npos)
            counter++; // is Hauseux ;) (see cg2->push..)

        return combined + (counter > 0 ? intToString(counter) : "");
    }

    class callGroups : public unitTest
    {
        public :

            callGroups () : unitTest () { };

            ~callGroups () {};

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                assertable<bool>::assertEqual((bool) _returnCode, true);

                typedef bool (*callback1_t) (string,string);
                typedef evias::core::callback<bool,string,callback1_t> callback1_class_t;
                typedef evias::core::callgroup<bool,string,callback1_t> callback1_group_t;

                // initialize callbacks
                callback1_class_t* cb_Equals  = new callback1_class_t(&cg1_stringEquals);
                callback1_class_t* cb_Bigger  = new callback1_class_t(&cg1_stringBigger);
                callback1_class_t* cb_Smaller = new callback1_class_t(&cg1_stringSmaller);

                callback1_group_t* cg1 = new callback1_group_t();
                cg1->push(cb_Equals, "hello", "hello");  // returns true
                cg1->push(cb_Equals, "hellO", "hello");  // returns false
                cg1->push(cb_Bigger, "helLo", "hello");  // returns true
                cg1->push(cb_Bigger, "hello", "helLo");  // returns false
                cg1->push(cb_Smaller, "hello", "helLo"); // returns true
                cg1->push(cb_Smaller, "helLo", "hello"); // returns false

                vector<bool> returns = cg1->run();

                assertable<int>::assertEqual(returns.size(), 6);
                assertable<bool>::assertEqual(returns[0], true);
                assertable<bool>::assertEqual(returns[1], false);
                assertable<bool>::assertEqual(returns[2], true);
                assertable<bool>::assertEqual(returns[3], false);
                assertable<bool>::assertEqual(returns[4], true);
                assertable<bool>::assertEqual(returns[5], false);

                // callback2_t: callback function signature
                // callback2_class_t: relative evias::core::callback instance
                // callback2_group_t: relative evias::core::callgroup instance
                typedef string (*callback2_t) (const char*, const char*);
                typedef evias::core::callback<string,const char*,callback2_t> callback2_class_t;
                typedef evias::core::callgroup<string,const char*,callback2_t> callback2_group_t;

                // could also call set_funcptr() after init but not need here.
                callback2_class_t* cb_display  = new callback2_class_t(&cg2_getDisplayName);
                callback2_class_t* cb_combine = new callback2_class_t(&cg2_getCombinedName);

                callback2_group_t* cg2 = new callback2_group_t();
                // init callback/params pair for cb_display
                cg2->push(cb_display, "Grégory", "Saive");
                cg2->push(cb_display, "Yannick", "Saive");
                cg2->push(cb_display, "Cynthia", "Saive");
                cg2->push(cb_display, "Claudine", "Hauseux");
                cg2->push(cb_display, "André", "Saive");

                // same params for cb_counter
                cg2->push(cb_combine, "Grégory", "Saive");
                cg2->push(cb_combine, "Yannick", "Saive");
                cg2->push(cb_combine, "Cynthia", "Saive");
                // upcoming call will produce (counter++) to execute
                // as we see a "Hauseux" (actually, as we don't see "Saive" ;))
                // so all upcoming results will have "1" appended (see decl)
                cg2->push(cb_combine, "Claudine", "Hauseux");
                cg2->push(cb_combine, "André", "Saive");
                cg2->push(cb_combine, "Rose", "Masson"); // counter++

                vector<string> returns2 = cg2->run();

                assertable<int>::assertEqual(returns2.size(), 11);

                // cb_display return tests
                assertable<string>::assertEqual(returns2[0], "Grégory Saive");
                assertable<string>::assertEqual(returns2[1], "Yannick Saive");
                assertable<string>::assertEqual(returns2[2], "Cynthia Saive");
                assertable<string>::assertEqual(returns2[3], "Claudine Hauseux");
                assertable<string>::assertEqual(returns2[4], "André Saive");

                // cb_combine return tests
                assertable<string>::assertEqual(returns2[5], "GrégorySaive");
                assertable<string>::assertEqual(returns2[6], "YannickSaive");
                assertable<string>::assertEqual(returns2[7], "CynthiaSaive");
                assertable<string>::assertEqual(returns2[8], "ClaudineHauseux1"); // 1 appended from here down..
                assertable<string>::assertEqual(returns2[9], "AndréSaive1");
                assertable<string>::assertEqual(returns2[10], "RoseMasson2");

                delete cg1;
                delete cg2;
                delete cb_Equals;
                delete cb_Bigger;
                delete cb_Smaller;
                delete cb_display;
                delete cb_combine;

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

