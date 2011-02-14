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
#include "../core/unit_test_suite.hpp"
#include "../application/console.hpp"

// unitary test classes include

#include "configFiles/objectParse.hpp"
#include "configFiles/objectWrite.hpp"
#include "configFiles/invalidRead.hpp"
#include "jsonObjects/simpleParse.hpp"
#include "jsonObjects/objectParse.hpp"
#include "jsonObjects/objectLoad.hpp"
#include "sqlObjects/dataChecks.hpp"
#include "sqlObjects/insertStmt.hpp"
#include "sqlObjects/removeStmt.hpp"
#include "sqlObjects/updateStmt.hpp"
#include "sqlObjects/paramsParse.hpp"
#include "qtViews/mainWnd.hpp"
#include "qtViews/enhancedView.hpp"
#include "databaseObjects/dbConnection.hpp"
#include "databaseObjects/dbInsert.hpp"
#include "databaseObjects/dbDelete.hpp"
#include "databaseObjects/dbUpdate.hpp"
#include "databaseObjects/dbFetchAll.hpp"
#include "networkObjects/packetObject.hpp"

#include <vector>
#include <string>

int main (int argc, char* args[])
{

    // XXX handle call arguments
    //  - should allow specifying test lists for execution
    //  - should allow configuration of tests

	using namespace std;
    using evias::core::test::unitTestSuite;
    using evias::core::test::testResult;
    using evias::application::consoleParser;
    using evias::core::in_vector;

    string project = "eVias C++ library unitary test suite";
    string usage = " \
        ./suite_execution.exe [--skip (config|json|sqlobjects|views|dbobjects|network)] \
                              [--only (config|json|sqlobjects|views|dbobjects|network)  \
    ";
    consoleParser* suiteCallArgs = new consoleParser(project, usage, argc, args);
    suiteCallArgs->canEmptyCall(true)
                 ->addAllowedArg("--skip")
                 ->addAllowedArg("--only")
                 ->parseAll();

    map<string,string> callArgs = suiteCallArgs->readData();

    // suite execution call configuration
    bool hasOnly = (callArgs.find("--only") != callArgs.end());
    bool hasSkip = (callArgs.find("--skip") != callArgs.end());

    bool testConfigFiles = false;
    bool testJson        = false;
    bool testSqlObjects  = false;
    bool testViews       = false;
    bool testDbObjects   = false;
    bool testNetwork     = false;

    map<string,int> validKeys;
    validKeys.insert(make_pair("config", 1));
    validKeys.insert(make_pair("json", 2));
    validKeys.insert(make_pair("sqlobjects", 3));
    validKeys.insert(make_pair("views", 4));
    validKeys.insert(make_pair("dbobjects", 5));
    validKeys.insert(make_pair("network", 6));

    if (hasOnly && callArgs["--only"].size() > 0 && (validKeys.find(callArgs["--only"]) != validKeys.end()) ) {
        string onlyVal = callArgs["--only"];

        if (onlyVal == "config") testConfigFiles = true;
        else if (onlyVal == "json") testJson = true;
        else if (onlyVal == "sqlobjects") testSqlObjects = true;
        else if (onlyVal == "views") testViews = true;
        else if (onlyVal == "dbobjects") testDbObjects = true;
        else if (onlyVal == "network") testNetwork = true;
    }
    else if (hasSkip && callArgs["--skip"].size() > 0) {
        // skip may have to split arguments with "," for several skipped modules
        string skipKeys = callArgs["--skip"];
        vector<string> keysToSkip;
        if (skipKeys.find(",") != string::npos) {
            // has multiple keys
            // (append last comma for in_vector() bug
            keysToSkip = evias::core::split(skipKeys.append(","), ',');
        }
        else keysToSkip.push_back(skipKeys);

        // for skip option, by default everything is tested
        testConfigFiles = testJson = testSqlObjects = testViews = testDbObjects = testNetwork = true;

        // foreach key found, "do not test"
        if (in_vector("config", keysToSkip))      testConfigFiles = false;
        if (in_vector("json", keysToSkip))        testJson        = false;
        if (in_vector("sqlobjects", keysToSkip))  testSqlObjects  = false;
        if (in_vector("views", keysToSkip))       testViews       = false;
        if (in_vector("dbobjects", keysToSkip))   testDbObjects   = false;
        if (in_vector("network", keysToSkip))     testNetwork     = false;
    }
    else {
        // no relevant arg, test everything
        testConfigFiles = testJson = testSqlObjects = testViews = testDbObjects = testNetwork = true;
    }

    // start unitary test suite configuration

    vector<string> params;

    unitTestSuite testSuite;

    //
    // initialize each unitary test service
    //

    evias::core::test::configFiles::objectParse* configFiles_objectParse = new evias::core::test::configFiles::objectParse();
    params.push_back ("/home/greg/srv/home.work/cpp/eviasLib/evias/tests/bin/config/configFiles/objectParse.ini");

    configFiles_objectParse->setOptions(params);
	configFiles_objectParse->setLabel("INI parsing");

	params.clear();

    evias::core::test::configFiles::objectWrite* configFiles_objectWrite = new evias::core::test::configFiles::objectWrite();
    params.push_back ("/home/greg/srv/home.work/cpp/eviasLib/evias/tests/bin/config/configFiles/objectWrite.ini");

    configFiles_objectWrite->setOptions(params);
	configFiles_objectWrite->setLabel("INI writing");

	params.clear();

    evias::core::test::configFiles::invalidRead* configFiles_invalidRead = new evias::core::test::configFiles::invalidRead();
    params.push_back ("/home/greg/srv/home.work/cpp/eviasLib/evias/tests/bin/config/configFiles/invalidRead.ini");

    configFiles_invalidRead->setOptions(params);
	configFiles_invalidRead->setLabel("INI invalid reading");

	params.clear();

    evias::core::test::jsonObjects::simpleParse* jsonObjects_simpleParse = new evias::core::test::jsonObjects::simpleParse();
	jsonObjects_simpleParse->setLabel("JSON classes simpleParse");

    evias::core::test::jsonObjects::objectParse* jsonObjects_objectParse = new evias::core::test::jsonObjects::objectParse();
	jsonObjects_objectParse->setLabel("JSON classes objectParse");

    evias::core::test::jsonObjects::objectLoad* jsonObjects_objectLoad = new evias::core::test::jsonObjects::objectLoad();
	jsonObjects_objectLoad->setLabel("JSON classes objectLoad");

    evias::core::test::sqlObjects::dataChecks* sqlObjects_dataChecks = new evias::core::test::sqlObjects::dataChecks();
    sqlObjects_dataChecks->setLabel("SQL classes query data checks");

    evias::core::test::sqlObjects::insertStmt* sqlObjects_insertStmt = new evias::core::test::sqlObjects::insertStmt();
	sqlObjects_insertStmt->setLabel("SQL classes insertStmt");

    evias::core::test::sqlObjects::removeStmt* sqlObjects_removeStmt = new evias::core::test::sqlObjects::removeStmt();
	sqlObjects_removeStmt->setLabel("SQL classes removeStmt");

    evias::core::test::sqlObjects::updateStmt* sqlObjects_updateStmt = new evias::core::test::sqlObjects::updateStmt();
	sqlObjects_updateStmt->setLabel("SQL classes updateStmt");

    evias::core::test::sqlObjects::paramsParse* sqlObjects_paramsParse = new evias::core::test::sqlObjects::paramsParse();
    sqlObjects_paramsParse->setLabel("SQL classes parameters parse (conditions)");

    evias::core::test::qtViews::mainWnd* qtViews_mainWnd = new evias::core::test::qtViews::mainWnd();
    qtViews_mainWnd->setLabel("Qt classes mainWnd");
    qtViews_mainWnd->setCall(argc,args);

    evias::core::test::qtViews::enhancedView* qtViews_enhancedView = new evias::core::test::qtViews::enhancedView();
    qtViews_enhancedView->setLabel("Qt classes enhancedView");
    qtViews_enhancedView->setCall(argc,args);

    params.clear();

    params.push_back("evias");
    params.push_back("developing");
    params.push_back("");
    params.push_back("web.evias.loc");

    evias::core::test::databaseObjects::dbConnection* databaseObjects_dbConnection = new evias::core::test::databaseObjects::dbConnection();
    databaseObjects_dbConnection->setOptions(params);
	databaseObjects_dbConnection->setLabel("Database objects dbConnection");

    evias::core::test::databaseObjects::dbInsert* databaseObjects_dbInsert = new evias::core::test::databaseObjects::dbInsert();
    databaseObjects_dbInsert->setOptions(params);
	databaseObjects_dbInsert->setLabel("Database objects dbInsert");

    evias::core::test::databaseObjects::dbDelete* databaseObjects_dbDelete = new evias::core::test::databaseObjects::dbDelete();
    databaseObjects_dbDelete->setOptions(params);
	databaseObjects_dbDelete->setLabel("Database objects dbDelete");

    evias::core::test::databaseObjects::dbUpdate* databaseObjects_dbUpdate = new evias::core::test::databaseObjects::dbUpdate();
    databaseObjects_dbUpdate->setOptions(params);
	databaseObjects_dbUpdate->setLabel("Database objects dbUpdate");

    evias::core::test::databaseObjects::dbFetchAll* databaseObjects_dbFetchAll = new evias::core::test::databaseObjects::dbFetchAll();
    databaseObjects_dbFetchAll->setOptions(params);
	databaseObjects_dbFetchAll->setLabel("Database objects dbFetchAll");

    evias::core::test::networkObjects::packetObject* networkObjects_packetObject = new evias::core::test::networkObjects::packetObject();
    networkObjects_packetObject->setLabel("Network objects packetObject");

/**
 * configure test suite
 * map a test object to a testResult.
 * result code will be checked, result message
 * will only in case message is not "no_message_check".
 **/

    if (testConfigFiles) {
        testSuite.addTest(configFiles_objectParse, testResult(1, "no_message_check"));
        testSuite.addTest(configFiles_objectWrite, testResult(1, "no_message_check"));
        testSuite.addTest(configFiles_invalidRead, testResult(1, "no_message_check"));
    }
    if (testJson) {
        testSuite.addTest(jsonObjects_simpleParse, testResult(1, "no_message_check"));
        testSuite.addTest(jsonObjects_objectParse, testResult(1, "no_message_check"));
        testSuite.addTest(jsonObjects_objectLoad, testResult(1, "no_message_check"));
    }
    if (testSqlObjects) {
        testSuite.addTest(sqlObjects_dataChecks, testResult(1, "no_message_check"));
        testSuite.addTest(sqlObjects_insertStmt, testResult(1, "no_message_check"));
        testSuite.addTest(sqlObjects_removeStmt, testResult(1, "no_message_check"));
        testSuite.addTest(sqlObjects_updateStmt, testResult(1, "no_message_check"));
        testSuite.addTest(sqlObjects_paramsParse, testResult(1, "no_message_check"));
    }
    if (testViews) {
        testSuite.addTest(qtViews_mainWnd, testResult(1, "no_message_check"));
        testSuite.addTest(qtViews_enhancedView, testResult(1, "no_message_check"));
    }
    if (testDbObjects) {
        testSuite.addTest(databaseObjects_dbConnection, testResult(1, "no_message_check"));
        testSuite.addTest(databaseObjects_dbInsert, testResult(1, "no_message_check"));
        testSuite.addTest(databaseObjects_dbDelete, testResult(1, "no_message_check"));
        testSuite.addTest(databaseObjects_dbUpdate, testResult(1, "no_message_check"));
        testSuite.addTest(databaseObjects_dbFetchAll, testResult(1, "no_message_check"));
    }
    if (testNetwork) {
        testSuite.addTest(networkObjects_packetObject, testResult(1, "no_message_check"));
    }

    testSuite.setQuietMode(false);
    testSuite.setOutputFile("/home/greg/srv/home.work/cpp/eviasLib/evias/tests/bin/logs/suite_execution_results.log");

    return (int) testSuite.execute();
}

