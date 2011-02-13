#include "unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

    unitTest::unitTest ()
    {
        // init timestamp
        time_t seed;
        time (&seed);
        srand ((unsigned long) seed);

        _startDate = evias::core::Date::now();

        _state = true;
        _returnMsg = "no_message_check";
        _label = "no label, date: " + _startDate->toString();

        _returnCode  = 3;
        _execSeconds = 0;
    }

    int unitTest::shutdown ()
    {
        _endDate = evias::core::Date::now();

        unsigned long totalExecTime = _endDate->timeStamp() - _startDate->timeStamp();

        int _execSeconds = totalExecTime;

        return _returnCode;
    }

}; // end namespace test

}; // end namespace core

}; // end namespace evias

