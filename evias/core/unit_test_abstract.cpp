#include "unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

    unitTest::unitTest ()
    {
        cout    << endl
                << "--- iSchool Unit Tester invoked."
                << endl;

        // init timestamp
        time_t seed;
        time (&seed);
        srand ((unsigned long) seed);

        _startDate = evias::core::Date::now();

        _state = true;
        _printMessage = "";
    }

    int unitTest::shutdown ()
    {
        _endDate = evias::core::Date::now();

        unsigned long totalExecTime = _endDate->timeStamp() - _startDate->timeStamp();

        int seconds = totalExecTime;

        cout    << endl
                << "--- Total execution time (in seconds) : " << seconds    << endl
                << "-- Date of Begin: " << _startDate->toString()           << endl
                << "-- Date of End : "  << _endDate->toString()             << endl
                << endl;

        return 0;
    }

}; // end namespace test

}; // end namespace core

}; // end namespace evias

