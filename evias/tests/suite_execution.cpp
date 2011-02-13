/**
 * Package: eVias Core unitary tests
 *
 * Copyright (c) 2010 - 2011 Grégory Saive
 *
 * For more informations about the licensing of this product, please refer
 * to the LICENCE file in the root application directory.
 *
 */

#include "../core/unit_test_suite.hpp"
#include "../application/console.hpp"

// unitary test classes include

#include "configFiles/objectParse.hpp"
#include "configFiles/objectWrite.hpp"
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

    string project = "eVias C++ library unitary test suite";
    string usage = "./suite_execution.exe [--no_config 1] [--no_json 1] [--no_sqlobj 1] [--no_views 1] [--no_dbobj 1]";
    consoleParser* suiteCallArgs = new consoleParser(project, usage, argc, args);
    suiteCallArgs->canEmptyCall(true)
                 ->addAllowedArg("--no_config")
                 ->addAllowedArg("--no_json")
                 ->addAllowedArg("--no_sqlobj")
                 ->addAllowedArg("--no_views")
                 ->addAllowedArg("--no_dbobj")
                 ->parseAll();

    map<string,string> callArgs = suiteCallArgs->readData();

    // suite execution call configuration
    bool testConfigFiles = (callArgs.find("--no_config") == callArgs.end());
    bool testJson        = (callArgs.find("--no_json") == callArgs.end());
    bool testSqlObjects  = (callArgs.find("--no_sqlobj") == callArgs.end());
    bool testViews       = (callArgs.find("--no_views") == callArgs.end());
    bool testDbObjects   = (callArgs.find("--no_dbobj") == callArgs.end());

    // start unitary test suite configuration

    vector<string> params;

    unitTestSuite testSuite;

    //
    // initialize each unitary test service
    //

	// begin test 1
    evias::core::test::configFiles::objectParse* configFiles_objectParse = new evias::core::test::configFiles::objectParse();
    params.push_back ("/home/greg/srv/home.work/cpp/eviasLib/evias/tests/bin/config/configFiles/objectParse.ini");

    configFiles_objectParse->setOptions(params);
	configFiles_objectParse->setLabel("INI parsing");
	// end test 1

	params.clear();

    // begin test 2
    evias::core::test::configFiles::objectWrite* configFiles_objectWrite = new evias::core::test::configFiles::objectWrite();
    params.push_back ("/home/greg/srv/home.work/cpp/eviasLib/evias/tests/bin/config/configFiles/objectWrite.ini");

    configFiles_objectWrite->setOptions(params);
	configFiles_objectWrite->setLabel("INI writing");
	// end test 2

	params.clear();

    // begin test 3
    evias::core::test::jsonObjects::simpleParse* jsonObjects_simpleParse = new evias::core::test::jsonObjects::simpleParse();
	jsonObjects_simpleParse->setLabel("JSON classes simpleParse");
	// end test 3

    // begin test 4
    evias::core::test::jsonObjects::objectParse* jsonObjects_objectParse = new evias::core::test::jsonObjects::objectParse();
	jsonObjects_objectParse->setLabel("JSON classes objectParse");
	// end test 4

    // begin test 5
    evias::core::test::jsonObjects::objectLoad* jsonObjects_objectLoad = new evias::core::test::jsonObjects::objectLoad();
	jsonObjects_objectLoad->setLabel("JSON classes objectLoad");
	// end test 5

    // begin test 6
    evias::core::test::sqlObjects::dataChecks* sqlObjects_dataChecks = new evias::core::test::sqlObjects::dataChecks();
    sqlObjects_dataChecks->setLabel("SQL classes query data checks");
    // end test 6

    // begin test 7
    evias::core::test::sqlObjects::insertStmt* sqlObjects_insertStmt = new evias::core::test::sqlObjects::insertStmt();
	sqlObjects_insertStmt->setLabel("SQL classes insertStmt");
	// end test 7

    // begin test 8
    evias::core::test::sqlObjects::removeStmt* sqlObjects_removeStmt = new evias::core::test::sqlObjects::removeStmt();
	sqlObjects_removeStmt->setLabel("SQL classes removeStmt");
	// end test 8

    // begin test 9
    evias::core::test::sqlObjects::updateStmt* sqlObjects_updateStmt = new evias::core::test::sqlObjects::updateStmt();
	sqlObjects_updateStmt->setLabel("SQL classes updateStmt");
	// end test 9

    // begin test 10
    evias::core::test::sqlObjects::paramsParse* sqlObjects_paramsParse = new evias::core::test::sqlObjects::paramsParse();
    sqlObjects_paramsParse->setLabel("SQL classes parameters parse (conditions)");
    // end test 10

    // begin test 11
    evias::core::test::qtViews::mainWnd* qtViews_mainWnd = new evias::core::test::qtViews::mainWnd();
    qtViews_mainWnd->setLabel("Qt classes mainWnd");
    qtViews_mainWnd->setCall(argc,args);
    // end test 12

    // begin test 12
    evias::core::test::qtViews::enhancedView* qtViews_enhancedView = new evias::core::test::qtViews::enhancedView();
    qtViews_enhancedView->setLabel("Qt classes enhancedView");
    qtViews_enhancedView->setCall(argc,args);
    // end test 12

    params.clear();

    // configuration for test 13 => 17
    params.push_back("evias");
    params.push_back("developing");
    params.push_back("");
    params.push_back("web.evias.loc");

    // begin test 13
    evias::core::test::databaseObjects::dbConnection* databaseObjects_dbConnection = new evias::core::test::databaseObjects::dbConnection();
    databaseObjects_dbConnection->setOptions(params);
	databaseObjects_dbConnection->setLabel("Database objects dbConnection");
	// end test 13

    // begin test 14
    evias::core::test::databaseObjects::dbInsert* databaseObjects_dbInsert = new evias::core::test::databaseObjects::dbInsert();
    databaseObjects_dbInsert->setOptions(params);
	databaseObjects_dbInsert->setLabel("Database objects dbInsert");
	// end test 14

    // begin test 15
    evias::core::test::databaseObjects::dbDelete* databaseObjects_dbDelete = new evias::core::test::databaseObjects::dbDelete();
    databaseObjects_dbDelete->setOptions(params);
	databaseObjects_dbDelete->setLabel("Database objects dbDelete");
	// end test 15

    // begin test 16
    evias::core::test::databaseObjects::dbUpdate* databaseObjects_dbUpdate = new evias::core::test::databaseObjects::dbUpdate();
    databaseObjects_dbUpdate->setOptions(params);
	databaseObjects_dbUpdate->setLabel("Database objects dbUpdate");
	// end test 16

    // begin test 17
    evias::core::test::databaseObjects::dbFetchAll* databaseObjects_dbFetchAll = new evias::core::test::databaseObjects::dbFetchAll();
    databaseObjects_dbFetchAll->setOptions(params);
	databaseObjects_dbFetchAll->setLabel("Database objects dbFetchAll");
	// end test 17

/**
 * configure test suite
 * map a test object to a testResult.
 * result code will be checked, result message
 * will only in case message is not "no_message_check".
 **/

    if (testConfigFiles) {
        testSuite.addTest(configFiles_objectParse, testResult(1, "no_message_check"));
        testSuite.addTest(configFiles_objectWrite, testResult(1, "no_message_check"));
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

    testSuite.setQuietMode(false);
    testSuite.setOutputFile("/home/greg/srv/home.work/cpp/eviasLib/evias/tests/bin/logs/suite_execution_results.log");

    return (int) testSuite.execute();
}

