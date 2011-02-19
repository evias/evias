/**
 * Package: eVias Core unitary tests
 *
 * Copyright (c) 2010 - 2011 Grégory Saive
 *
 * For more informations about the licensing of this product, please refer
 * to the LICENCE file in the root application directory.
 *
 */

#include "../core/string_utils.hpp"
#include "../application/console.hpp"
#include "library_test_suite.hpp"

// unitary test classes include


#include <vector>
#include <string>

int main (int argc, char* args[])
{

	using namespace std;
    using evias::core::test::eviasTestSuite;
    using evias::core::test::testResult;
    using evias::application::consoleParser;
    using evias::core::in_vector;

    string project = "eVias C++ library unitary test suite";
    string usage = " \
        ./suite_execution.exe [--skip config,json,sqlobjects,views,dbobjects,network,regexp] \
                              [--only config,json,sqlobjects,views,dbobjects,network,regexp]  \
    ";
    consoleParser* suiteCallArgs = new consoleParser(project, usage, argc, args);
    suiteCallArgs->canEmptyCall(true)
                 ->addAllowedArg("--skip")
                 ->addAllowedArg("--only")
                 ->parseAll();

    map<string,string> callArgs = suiteCallArgs->readData();

    // suite execution call configuration

    // basically we test everything.
    bool testConfigFiles = true;
    bool testJson        = true;
    bool testSqlObjects  = true;
    bool testViews       = true;
    bool testDbObjects   = true;
    bool testNetwork     = true;
    bool testRegExp      = true;

    bool hasOnly = (callArgs.find("--only") != callArgs.end());
    bool hasSkip = (callArgs.find("--skip") != callArgs.end());

    // callArg[KEY] access facility
    string usingOption = "default";
    if (hasOnly && callArgs["--only"].size() > 0) {
        usingOption = "--only";
    }
    else if (hasSkip && callArgs["--skip"].size() > 0) {
        usingOption = "--skip";
    }

    // process call arguments
    if (usingOption == "--only" || usingOption == "--skip") {
        // we have a workable option (size > 0)

        vector<string> optionKeys;
        string optionData = callArgs[usingOption];

        evias::core::trim(optionData, " ");
        if (optionData.find(",") != string::npos)
            optionKeys = evias::core::split(optionData.append(","), ','); // multi key ',' separated
        else
            optionKeys.push_back(optionData); // single key

        // if we are in "skip mode" => we test everything except present key(s)
        // if we are in "only mode" => we test nothing but present key(s)
        bool initTest = true; // everything
        if (usingOption == "--only") {
            initTest = false;
        }

        testConfigFiles = testJson = testSqlObjects = testViews = testDbObjects = testNetwork = testRegExp = (initTest);

        // present in option key(s) means having to change the data
        if (in_vector("config", optionKeys))     testConfigFiles  = ! initTest;
        if (in_vector("json", optionKeys))       testJson         = ! initTest;
        if (in_vector("sqlobjects", optionKeys)) testSqlObjects   = ! initTest;
        if (in_vector("views", optionKeys))      testViews        = ! initTest;
        if (in_vector("dbobjects", optionKeys))  testDbObjects    = ! initTest;
        if (in_vector("network", optionKeys))    testNetwork      = ! initTest;
        if (in_vector("regexp", optionKeys))     testRegExp       = ! initTest;
    }

    // configure the test suite
    eviasTestSuite* librarySuite = new eviasTestSuite(argc, args);

    librarySuite->setTestConfig(testConfigFiles)
                ->setTestJSON(testJson)
                ->setTestSQL(testSqlObjects)
                ->setTestViews(testViews)
                ->setTestDatabase(testDbObjects)
                ->setTestNetwork(testNetwork)
                ->setTestRegExp(testRegExp);

    // execute tests
    int returnCode = librarySuite->execute();

    delete librarySuite;

    return returnCode;
}

