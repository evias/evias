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

    bool unitTest::isFillingDependencies()
    {
        if (! hasDependence())
            return true;

        bool valid = true;
        for (vector<unitTest*>::iterator dep = _dependsOn.begin();
            dep != _dependsOn.end();
            dep++) {

            // execute dependence unitary test
            (*dep)->prepare();
            (*dep)->execute();
            int    dependenceCode = (*dep)->shutdown();
            string dependenceMsg  = (*dep)->getMessage();
            testResult depExecResult(dependenceCode, dependenceMsg);

            bool hasResult = _dependenceExpectedResults.find((*dep)->getLabel()) != _dependenceExpectedResults.end();

            if (hasResult) {
                // compare gotten result with expected one
                valid = valid && _dependenceExpectedResults[(*dep)->getLabel()].isValidResult(depExecResult);
            }
            else valid = false;
        }

        return valid;
    }

    void unitTest::addDependence(unitTest* t, testResult r)
    {
        _dependsOn.push_back(t);
        _dependenceExpectedResults.insert(pair<string,testResult>(t->getLabel(), r));
    }

}; // end namespace test

}; // end namespace core

}; // end namespace evias

