#ifndef __EVIAS_CORE_TEST_NS_UNIT_TEST_SUITE__
#define __EVIAS_CORE_TEST_NS_UNIT_TEST_SUITE__

#include <vector>
#include <map>
#include <string>
#include <fstream>

#include "string_utils.hpp"
#include "unit_test_abstract.hpp"

namespace evias {

namespace core {

namespace test {

    using namespace std;

	using evias::core::intToString;

    class testResult
    {
        public :

			testResult()
				: _returnCode(0), _returnMsg("default")
			{}

            testResult(int code, string msg = "no_message_check")
                : _returnCode(code), _returnMsg(msg)
            {}

            string getResult()
                { return "Code : " + intToString(_returnCode) + ". Message : " + _returnMsg; };
            int getCode()
                { return _returnCode; };
            string getMessage()
                { return _returnMsg; };

            int getReturnCode()
                { return _returnCode; };

            bool isValidCode(int code)
                { return _returnCode == code; };

            bool isValidMsg(string msg)
                { return _returnMsg == msg; };

			bool isValidResult(testResult rlt)
			{
				return rlt.getCode() == _returnCode && (_returnMsg == "no_message_check" || rlt.getMessage() == _returnMsg);
			}

        private :

            int     _returnCode;
            string  _returnMsg; // may be empty
    };

    class unitTestSuite
    {
        public :

            unitTestSuite(bool = true);
            ~unitTestSuite();

            bool execute();

            int addTest(unitTest*, testResult);

            unitTestSuite* const chain(unitTest*, testResult);

            vector<unitTest*> getTests()
                { return _tests; };
            int getCount()
                { return _count; };

            void setOutputFile(string filePath)
                { _outputFile = filePath; };

            void setErrorFile(string filePath)
                { _errorFile = filePath; };

            void setQuietMode(bool isQuiet = true)
                { _beQuiet = isQuiet; };

			void printResult(string, testResult, testResult, bool = false);
            void notify(string);
            void error(string);

        private :

            int     _count;
            bool    _beQuiet;
            string  _outputFile;
            string  _errorFile;

            vector<unitTest*> _tests;

            map<int, testResult> _expectedResults;
            map<int, testResult> _testResults;
    };

}; // test

}; // core

}; // evias

#endif