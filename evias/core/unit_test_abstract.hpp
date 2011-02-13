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

    typedef enum {
        RETURN_SUCCESS      = 1,
    } serviceReturnCode;

    typedef enum {
        ERROR_DATA_INPUT    = -1,
        ERROR_ENVIRONMENT   = -2,
        ERROR_DEVELOPMENT   = -3,
        ERROR_TEST_DATA     = -4
    } serviceErrorCode;

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

            virtual void prepare () = 0;
            virtual int execute () = 0;

            int shutdown ();

            void setOptions(vector<string> opts)
                { _options = opts; };
            int setReturnCode(int code)
                { _returnCode = code; return _returnCode; };
            void setMessage(string msg)
                { _returnMsg = msg; };
            void setLabel(string label)
                { _label = label; };

            int getPastSeconds()
                { return _execSeconds; };
            int getReturnCode()
                { return _returnCode; };
            string getMessage()
                { return _returnMsg; };
            vector<string> getOptions()
                { return _options; };
            string getLabel()
                { return _label; };

        protected :
            bool _state;

            string _label;

            string _returnMsg;
            int _returnCode;
            int _execSeconds;

            vector<string> _options;

            evias::core::Date* _startDate;
            evias::core::Date* _endDate;
    };

}; // end namespace test

}; // end namespace core

}; // end namespace evias

#endif // __EVIAS_CORE_TEST_NS_UNIT_TEST__

