#include "library_test_suite.hpp"

namespace evias {

namespace core {

namespace test {

    eviasTestSuite::eviasTestSuite(int argc, char** argv)
        : _argc(argc),
          _args(argv)
    {
        unitTestSuite(false);

        setOutputFile("/home/greg/srv/home.work/cpp/eviasLib/evias/tests/bin/logs/suite_execution_results.log");

        // XXX read from config

        _initTestsConfig();
        _initTestsJSON();
        _initTestsSQL();
        _initTestsViews();
        _initTestsDatabase();
        _initTestsNetwork();
        _initTestsRegExp();
    }

    eviasTestSuite::~eviasTestSuite()
    {
        if (_testConfig) {
            releaseMemory(_configFiles_objectParse);
            releaseMemory(_configFiles_objectWrite);
            releaseMemory(_configFiles_invalidRead);
            releaseMemory(_configFiles_commentedFile);
        }

        if (_testJSON) {
            releaseMemory(_jsonObjects_simpleParse);
            releaseMemory(_jsonObjects_objectParse);
            releaseMemory(_jsonObjects_objectLoad);
        }

        if (_testSQL) {
            releaseMemory(_sqlObjects_dataChecks);
            releaseMemory(_sqlObjects_insertStmt);
            releaseMemory(_sqlObjects_removeStmt);
            releaseMemory(_sqlObjects_updateStmt);
            releaseMemory(_sqlObjects_paramsParse);
        }

        if (_testViews) {
            releaseMemory(_qtViews_mainWnd);
            releaseMemory(_qtViews_enhancedView);
        }

        if (_testDatabase) {
            releaseMemory(_databaseObjects_dbConnection);
            releaseMemory(_databaseObjects_dbInsert);
            releaseMemory(_databaseObjects_dbDelete);
            releaseMemory(_databaseObjects_dbUpdate);
            releaseMemory(_databaseObjects_dbFetchAll);
        }

        if (_testNetwork) {
            releaseMemory(_networkObjects_packetObject);
        }

        if (_testRegExp) {
            releaseMemory(_regularExpressions_charsRanges);
            releaseMemory(_regularExpressions_easyParse);
        }
    }

    eviasTestSuite* const eviasTestSuite::setTestConfig(bool test)
    {
        if ((_testConfig = test)) {
            addTest(_configFiles_objectParse, testResult(1, "no_message_check"));
            addTest(_configFiles_objectWrite, testResult(1, "no_message_check"));
            addTest(_configFiles_invalidRead, testResult(1, "no_message_check"));
            addTest(_configFiles_commentedFile, testResult(1, "no_message_check"));
        }

        return this;
    }

    eviasTestSuite* const eviasTestSuite::setTestJSON(bool test)
    {
        if ((_testJSON = test)) {
            addTest(_jsonObjects_simpleParse, testResult(1, "no_message_check"));
            addTest(_jsonObjects_objectParse, testResult(1, "no_message_check"));
            addTest(_jsonObjects_objectLoad, testResult(1, "no_message_check"));
        }

        return this;
    }

    eviasTestSuite* const eviasTestSuite::setTestSQL(bool test)
    {
        if ((_testSQL = test)) {
            addTest(_sqlObjects_dataChecks, testResult(1, "no_message_check"));
            addTest(_sqlObjects_insertStmt, testResult(1, "no_message_check"));
            addTest(_sqlObjects_removeStmt, testResult(1, "no_message_check"));
            addTest(_sqlObjects_updateStmt, testResult(1, "no_message_check"));
            addTest(_sqlObjects_paramsParse, testResult(1, "no_message_check"));
        }
        return this;
    }

    eviasTestSuite* const eviasTestSuite::setTestViews(bool test)
    {
        if ((_testViews = test)) {
            addTest(_qtViews_mainWnd, testResult(1, "no_message_check"));
            addTest(_qtViews_enhancedView, testResult(1, "no_message_check"));
        }

        return this;
    }

    eviasTestSuite* const eviasTestSuite::setTestDatabase(bool test)
    {
        if ((_testDatabase = test)) {
            addTest(_databaseObjects_dbConnection, testResult(1, "no_message_check"));
            addTest(_databaseObjects_dbInsert, testResult(1, "no_message_check"));
            addTest(_databaseObjects_dbDelete, testResult(1, "no_message_check"));
            addTest(_databaseObjects_dbUpdate, testResult(1, "no_message_check"));
            addTest(_databaseObjects_dbFetchAll, testResult(1, "no_message_check"));
        }
        return this;
    }

    eviasTestSuite* const eviasTestSuite::setTestNetwork(bool test)
    {
        if ((_testNetwork = test)) {
            addTest(_networkObjects_packetObject, testResult(1, "no_message_check"));
        }
        return this;
    }

    eviasTestSuite* const eviasTestSuite::setTestRegExp(bool test)
    {
        if ((_testRegExp = test)) {
            addTest(_regularExpressions_charsRanges, testResult(1, "no_message_check"));
            addTest(_regularExpressions_easyParse, testResult(1, "no_message_check"));
        }
        return this;
    }


    void eviasTestSuite::_initTestsConfig()
    {
        vector<string> params;

        _configFiles_objectParse = new configFiles::objectParse();
        params.push_back ("/home/greg/srv/home.work/cpp/eviasLib/evias/tests/bin/config/configFiles/objectParse.ini");

        _configFiles_objectParse->setOptions(params);
        _configFiles_objectParse->setLabel("INI parsing");

        params.clear();

        _configFiles_objectWrite = new configFiles::objectWrite();
        params.push_back ("/home/greg/srv/home.work/cpp/eviasLib/evias/tests/bin/config/configFiles/objectWrite.ini");

        _configFiles_objectWrite->setOptions(params);
        _configFiles_objectWrite->setLabel("INI writing");

        params.clear();

        _configFiles_invalidRead = new configFiles::invalidRead();
        params.push_back ("/home/greg/srv/home.work/cpp/eviasLib/evias/tests/bin/config/configFiles/invalidRead.ini");

        _configFiles_invalidRead->setOptions(params);
        _configFiles_invalidRead->setLabel("INI invalid reading");

        params.clear();

        _configFiles_commentedFile = new configFiles::commentedFile();
        params.push_back ("/home/greg/srv/home.work/cpp/eviasLib/evias/tests/bin/config/configFiles/commentedFile.ini");

        _configFiles_commentedFile->setOptions(params);
        _configFiles_commentedFile->setLabel("INI file with comments");

        params.clear();
    }

    void eviasTestSuite::_initTestsJSON()
    {
        _jsonObjects_simpleParse = new jsonObjects::simpleParse();
        _jsonObjects_simpleParse->setLabel("JSON classes simpleParse");

        _jsonObjects_objectParse = new jsonObjects::objectParse();
        _jsonObjects_objectParse->setLabel("JSON classes objectParse");

        _jsonObjects_objectLoad = new jsonObjects::objectLoad();
        _jsonObjects_objectLoad->setLabel("JSON classes objectLoad");
    }

    void eviasTestSuite::_initTestsSQL()
    {
        _sqlObjects_dataChecks = new sqlObjects::dataChecks();
        _sqlObjects_dataChecks->setLabel("SQL classes query data checks");

        _sqlObjects_insertStmt = new sqlObjects::insertStmt();
        _sqlObjects_insertStmt->setLabel("SQL classes insertStmt");

        _sqlObjects_removeStmt = new sqlObjects::removeStmt();
        _sqlObjects_removeStmt->setLabel("SQL classes removeStmt");

        _sqlObjects_updateStmt = new sqlObjects::updateStmt();
        _sqlObjects_updateStmt->setLabel("SQL classes updateStmt");

        _sqlObjects_paramsParse = new sqlObjects::paramsParse();
        _sqlObjects_paramsParse->setLabel("SQL classes parameters parse (conditions)");

    }

    void eviasTestSuite::_initTestsViews()
    {
        _qtViews_mainWnd = new qtViews::mainWnd();
        _qtViews_mainWnd->setLabel("Qt classes mainWnd");
        _qtViews_mainWnd->setCall(_argc,_args);

        _qtViews_enhancedView = new qtViews::enhancedView();
        _qtViews_enhancedView->setLabel("Qt classes enhancedView");
        _qtViews_enhancedView->setCall(_argc,_args);
    }

    void eviasTestSuite::_initTestsDatabase()
    {
        vector<string> params;

        params.push_back("evias");
        params.push_back("developing");
        params.push_back("");
        params.push_back("web.evias.loc");

        _databaseObjects_dbConnection = new databaseObjects::dbConnection();
        _databaseObjects_dbConnection->setOptions(params);
        _databaseObjects_dbConnection->setLabel("Database objects dbConnection");

        _databaseObjects_dbInsert = new databaseObjects::dbInsert();
        _databaseObjects_dbInsert->setOptions(params);
        _databaseObjects_dbInsert->setLabel("Database objects dbInsert");

        _databaseObjects_dbDelete = new databaseObjects::dbDelete();
        _databaseObjects_dbDelete->setOptions(params);
        _databaseObjects_dbDelete->setLabel("Database objects dbDelete");

        _databaseObjects_dbUpdate = new databaseObjects::dbUpdate();
        _databaseObjects_dbUpdate->setOptions(params);
        _databaseObjects_dbUpdate->setLabel("Database objects dbUpdate");

        _databaseObjects_dbFetchAll = new databaseObjects::dbFetchAll();
        _databaseObjects_dbFetchAll->setOptions(params);
        _databaseObjects_dbFetchAll->setLabel("Database objects dbFetchAll");
    }

    void eviasTestSuite::_initTestsNetwork()
    {
        _networkObjects_packetObject = new networkObjects::packetObject();
        _networkObjects_packetObject->setLabel("Network objects packetObject");
    }

    void eviasTestSuite::_initTestsRegExp()
    {
        _regularExpressions_charsRanges = new regularExpressions::charsRanges();
        _regularExpressions_charsRanges->setLabel("eRegExp' charsRanges class");

        _regularExpressions_easyParse = new regularExpressions::easyParse();
        _regularExpressions_easyParse->setLabel("eRegExp' easyParse class");
    }


}; // test

}; // core

}; // evias

