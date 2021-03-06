#ifndef __EVIAS_CORE_TEST_NS_LIBRARY_TEST_SUITE__
#define __EVIAS_CORE_TEST_NS_LIBRARY_TEST_SUITE__

#include <vector>
#include <map>
#include <string>
#include <fstream>

#include "../core/unit_test_suite.hpp"

#include "configFiles/objectParse.hpp"
#include "configFiles/objectWrite.hpp"
#include "configFiles/invalidRead.hpp"
#include "configFiles/commentedFile.hpp"
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
#include "networkObjects/ircClient.hpp"
#include "regularExpressions/indexedResult.hpp"
#include "regularExpressions/errorCodes.hpp"
#include "regularExpressions/namedGroups.hpp"
#include "regularExpressions/furtherGroups.hpp"
#include "regularExpressions/boostRegex.hpp"
#include "functional/functorTest.hpp"
#include "functional/moreFunctor.hpp"
#include "functional/callGroups.hpp"

namespace evias {

namespace core {

namespace test {

    class eviasTestSuite : public unitTestSuite
    {
    public :

        eviasTestSuite();
        virtual ~eviasTestSuite();

        eviasTestSuite* const setTestConfig(bool);
        eviasTestSuite* const setTestJSON(bool);
        eviasTestSuite* const setTestSQL(bool);
        eviasTestSuite* const setTestViews(bool);
        eviasTestSuite* const setTestDatabase(bool);
        eviasTestSuite* const setTestNetwork(bool);
        eviasTestSuite* const setTestRegExp(bool);
        eviasTestSuite* const setTestFunctional(bool);

    protected :

        void _initTestsConfig();
        void _initTestsJSON();
        void _initTestsSQL();
        void _initTestsViews();
        void _initTestsDatabase();
        void _initTestsNetwork();
        void _initTestsRegExp();
        void _initTestsFunctional();

    private :

        int _argc;
        char** _args;

        bool _testConfig;
        bool _testJSON;
        bool _testSQL;
        bool _testViews;
        bool _testDatabase;
        bool _testNetwork;
        bool _testRegExp;
        bool _testFunctional;

        configFiles::objectParse*   _configFiles_objectParse;
        configFiles::objectWrite*   _configFiles_objectWrite;
        configFiles::invalidRead*   _configFiles_invalidRead;
        configFiles::commentedFile* _configFiles_commentedFile;

        jsonObjects::simpleParse*   _jsonObjects_simpleParse;
        jsonObjects::objectParse*   _jsonObjects_objectParse;
        jsonObjects::objectLoad*    _jsonObjects_objectLoad;

        sqlObjects::dataChecks*     _sqlObjects_dataChecks;
        sqlObjects::insertStmt*     _sqlObjects_insertStmt;
        sqlObjects::removeStmt*     _sqlObjects_removeStmt;
        sqlObjects::updateStmt*     _sqlObjects_updateStmt;
        sqlObjects::paramsParse*    _sqlObjects_paramsParse;

        qtViews::mainWnd*           _qtViews_mainWnd;
        qtViews::enhancedView*      _qtViews_enhancedView;

        databaseObjects::dbConnection*  _databaseObjects_dbConnection;
        databaseObjects::dbInsert*      _databaseObjects_dbInsert;
        databaseObjects::dbDelete*      _databaseObjects_dbDelete;
        databaseObjects::dbUpdate*      _databaseObjects_dbUpdate;
        databaseObjects::dbFetchAll*    _databaseObjects_dbFetchAll;

        networkObjects::packetObject*   _networkObjects_packetObject;
        networkObjects::ircClient*      _networkObjects_ircClient;

        regularExpressions::errorCodes*     _regularExpressions_errorCodes;
        regularExpressions::indexedResult*  _regularExpressions_indexedResult;
        regularExpressions::boostRegex*     _regularExpressions_boostRegex;
        regularExpressions::namedGroups*    _regularExpressions_namedGroups;
        regularExpressions::furtherGroups*  _regularExpressions_furtherGroups;

        functional::functorTest*        _functional_functorTest;
        functional::moreFunctor*        _functional_moreFunctor;
        functional::callGroups*         _functional_callGroups;
    };

}; // test

}; // core

}; // evias

#endif
