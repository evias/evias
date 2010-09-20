#ifndef __EVIAS_CORE_TEST_NS_UNIT_TEST__
#define __EVIAS_CORE_TEST_NS_UNIT_TEST__

#include <string>
#include <vector>
#include <typeinfo>
#include <iostream>
#include <ctime>
#include <cstdlib>

#include "date.hpp"

namespace evias {

namespace core {

namespace test {

    using namespace std;

    class unitTest
    {
        public :
            unitTest ();
            ~unitTest() {
                if (_startDate != NULL)
                    delete _startDate;

                if (_endDate != NULL)
                    delete _endDate;
            };

            virtual int prepare (vector<string> params) = 0;
            virtual int execute () = 0;

            int shutdown ();

        protected :
            string _printMessage;
            bool _state;

            evias::core::Date* _startDate;
            evias::core::Date* _endDate;
    };

}; // end namespace test

}; // end namespace core

}; // end namespace evias

#endif // __EVIAS_CORE_TEST_NS_UNIT_TEST__

