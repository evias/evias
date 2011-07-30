#include "unit_test_suite.hpp"

namespace evias {

namespace core {

namespace test {

    unitTestSuite::unitTestSuite(bool beQuiet)
     : _count(0),
       _outputFile(""),
       _verbosity(VERBOSE)
    {
    }

    unitTestSuite::unitTestSuite(const unitTestSuite & copy)
     : _count(copy._count),
       _outputFile(copy._outputFile),
       _verbosity(copy._verbosity),
       _tests(copy._tests),
       _expectedResults(copy._expectedResults),
       _testResults(copy._testResults)
    {
    }

    unitTestSuite::~unitTestSuite()
    {
    }

    unitTestSuite* const unitTestSuite::setVerbosity(unitTestVerbosity v)
    {
        _verbosity = v;

        return this;
    }

    bool unitTestSuite::execute()
    {
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

        string backColor= "\[\033[0m";
        string msgColor = "\[\033[32m"; // green
        if (! awaited.isValidResult(result)) {
            msgColor = "\[\033[31m"; // red
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

    void unitTestSuite::notify(string msg, bool force)
    {
        if (! force && _verbosity == QUIET) return;

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
