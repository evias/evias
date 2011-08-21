#include "unit_test_suite.hpp"
#include "../application/console.hpp"

namespace evias {

namespace core {

namespace test {

    using evias::application::consoleParser;
    using evias::application::Project;

    unitTestSuite::unitTestSuite()
     : _count(0),
       _outputFile(""),
       _verbosity(VERBOSE),
       Project("unitary test suite application")
    {
    }

    unitTestSuite::unitTestSuite(const unitTestSuite & copy)
     : _count(copy._count),
       _outputFile(copy._outputFile),
       _verbosity(copy._verbosity),
       _tests(copy._tests),
       _expectedResults(copy._expectedResults),
       _testResults(copy._testResults),
       Project(copy._name)
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

    int unitTestSuite::bootstrap(int argc, char** argv)
    {
        _consoleParser = new evias::application::consoleParser(argc, argv);

        return 1;
    }

    int unitTestSuite::execute()
    {
        int execReturn = 1;
        int cntSuccess  = 0;
        int cntFailures = 0;

        vector<string> displayedGroups;
        for (int i = 0, max = _tests.size(); i < max; i++) {

            if (! in_vector(_groups[i], displayedGroups)) {
                displayNewTestsGroup(_groups[i]);

                displayedGroups.push_back(_groups[i]);
            }

            int currentTestCode   = -1;
            string currentTestMsg = "no_message_check";

            if (_tests[i]->hasDependence() && ! _tests[i]->isFillingDependencies())
                // dependencies check failed
                currentTestCode = DEPENDENCE_FAILURE;
            else {

                // prepare, execute test, get after execution info
                _tests[i]->prepare();
                _tests[i]->execute();
                currentTestCode = _tests[i]->shutdown();
                currentTestMsg  = _tests[i]->getMessage();
            }

            // save result
            testResult currentResult(currentTestCode, currentTestMsg);
            _testResults.insert(make_pair(i, currentResult));

            // return boolean may change here
            execReturn = currentTestCode != SUCCESS ? currentTestCode : execReturn;

            // print to console + file if specified
            switch (_verbosity)
            {
                default :
                case VERBOSE :
                    printResult(_tests[i]->getLabel(), currentResult, _expectedResults[i], i==0);
                    break;

                case NORMAL :
                    shortResult(_tests[i]->getLabel(), currentResult, _expectedResults[i]);
                    break;

                case QUIET :
                    break;
            }
            
            if (_expectedResults[i].isValidResult(currentResult))
                cntSuccess++;
            else
                cntFailures++;
        }

        notify("");
        notify("");

        colorNotify(
            string("#### execution of unitary test suite done with ")
            .append(intToString(cntSuccess)).append(" sucess(es) and ")
            .append(intToString(cntFailures)).append(" failures.")
            .append(" Return code is: ").append(execReturn ? "1" : "0"),
            (cntFailures == 0),
            true
        );

        return execReturn;
    }

    int unitTestSuite::shutdown()
    {
        _tests.clear();
        _expectedResults.clear();
        _testResults.clear();

        return 1;
    }

    int unitTestSuite::addTest(unitTest* test, testResult awaited, string group)
    {
        _tests.push_back(test);
        _expectedResults.insert(make_pair(_count, awaited));

        _groups.push_back(group);

        _count++;

        return _count;
    }

    unitTestSuite* const unitTestSuite::chain(unitTest* test, testResult awaited, string group)
    {
        addTest(test, awaited, group);

        return this;
    }

    void unitTestSuite::colorNotify(string msg, bool success, bool force)
    {
        if (! force && _verbosity == QUIET)
            return;
        
        string backColor= "\[\e[1;0m]";
        string msgColor = "\[\e[1;32m]"; // green
        if (! success) {
            msgColor = "\[\e[1;31m]"; // red
        }

        cout << msgColor << msg << backColor << endl;
    }

    void unitTestSuite::displayNewTestsGroup(string group)
    {
        string backColor = "\[\e[1;0m]";
        string msgColor  = "\[\e[1;35m]"; // blue
        string hitEnter  = "\[\e[1;31m]"; // red (require action)

        cout << endl << msgColor << "NOW EXECUTING TESTS GROUP '"
             << group << "'" << endl
             << "------------------------------------------"
             << hitEnter << " Please hit the Enter key to start." << backColor;

        cin.get();
        cout << endl;
    }

    void unitTestSuite::shortResult(string label, testResult result, testResult awaited)
    {
        string backColor= "\[\e[1;0m]";
        string msgColor = "\[\e[1;32m]"; // green
        if (! awaited.isValidResult(result)) {
            msgColor = "\[\e[1;31m]"; // red
        }

        string errorMsg = string("[Code: ")
                        .append(intToString(result.getCode()))
                        .append("]");

        cout << msgColor << "--- TEST RESULT for '" << label << "' is : ["
             << (awaited.isValidResult(result) ? "RESULT MATCH" : string("RESULT ERROR ").append(errorMsg))
             << "]"
             << backColor
             << endl;
    }

    void unitTestSuite::printResult(string label, testResult result, testResult awaited, bool flushBefore)
    {
        // XXX error messages should be written in new file {_outputFile}_error
        string backColor= "\[\e[1;0m]";
        string msgColor = "\[\e[1;32m]"; // green
        if (! awaited.isValidResult(result)) {
            msgColor = "\[\e[1;31m]"; // red
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
