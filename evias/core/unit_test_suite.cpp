#include "unit_test_suite.hpp"

namespace evias {

namespace core {

namespace test {

    unitTestSuite::unitTestSuite(bool beQuiet)
     : _count(0),
       _beQuiet(beQuiet),
       _outputFile("")
    {
    }

    unitTestSuite::~unitTestSuite()
    {
    }

    bool unitTestSuite::execute()
    {
        notify("#### eVias unitary test suite will now execute all added tests.");
        notify("");
        notify("");

        bool execReturn = true;
        for (int i = 0, max = _tests.size(); i < max; i++) {

            int currentTestCode   = -1;
            string currentTestMsg = "no_message_check";

            // prepare, execute test, get after execution info
            _tests[i]->prepare();
            _tests[i]->execute();
            currentTestCode = _tests[i]->shutdown();
            currentTestMsg  = _tests[i]->getMessage();

            // save result
            testResult currentResult(currentTestCode, currentTestMsg);
            _testResults.insert(make_pair(i, currentResult));

            // return boolean may change here
            execReturn = execReturn && _expectedResults[i].isValidCode(currentTestCode);

            // print to console + file if specified
            printResult(_tests[i]->getLabel(), currentResult, _expectedResults[i], i==0);
        }

        notify("");
        notify("");
        notify(string("#### execution of eVias unitary test suite done, return code : ").append(execReturn ? "1" : "0"));

        return execReturn;
    }

    int unitTestSuite::addTest(unitTest* test, testResult awaited)
    {
        _tests.push_back(test);
        _expectedResults.insert(make_pair(_count, awaited));

        _count++;

        return _count;
    }

    unitTestSuite* const unitTestSuite::chain(unitTest* test, testResult awaited)
    {
        addTest(test, awaited);

        return this;
    }

    void unitTestSuite::printResult(string label, testResult result, testResult awaited, bool flushBefore)
    {
        // XXX error messages should be written in new file {_outputFile}_error

        string backColor= "\[\033[00;0m";
        string msgColor = "\[\033[01;32m"; // green
        if (! awaited.isValidResult(result)) {
            msgColor = "\[\033[01;31m"; // red
        }

        cout << msgColor
             << endl
             << "#### Results for test '" << label << "'" << " is : "
             << (awaited.isValidResult(result) ? "RESULT MATCH" : "RESULT ERROR")
             << endl << "-- Expected : "
             << awaited.getResult()
             << endl << "-- Execution Result : "
             << result.getResult()
             << endl
             << "#### End result of '" << label << "'"
             << endl
             << backColor
             << endl;

        if (_outputFile.size() > 0) {
            ofstream s;
            if (flushBefore) {
                s.open(_outputFile.c_str(), ios::out);
            }
            else {
                s.open(_outputFile.c_str(), ios::app);
            }

            if (! s.good()) {
                return ;
            }

            s << "---- Result for test '" << label << "'"
              << endl
              << endl << "-- Expected : "
              << awaited.getResult()
              << endl << "-- Execution Result : "
              << result.getResult()
              << endl << endl;

            s.close();
        }
    }

    void unitTestSuite::notify(string msg)
    {
        if (_beQuiet) return;

        cout << msg << endl;
    }

    void unitTestSuite::error(string msg)
    {
        if (_errorFile.size() > 0) {
            ofstream s;
            s.open(_errorFile.c_str(), ios::out);
            if (! s.good()) {
                return ;
            }

            s << msg
              << endl;

            s.close();
        }
    }

}; // test

}; // core

}; // evias