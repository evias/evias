/**
 * Package: 	iSchool Core Unit Testing
 *
 * Implemented Namespaces:
 * 	- evias::core::test
 *
 * Implemented Classes:
 *  - mainWnd_Full
 *
 * Code Description:
 *  - tests configIni's parse method
 *
 * Copyright (c) 2010 - 2011 Grégory Saive
 *
 * For more informations about the licensing of this product, please refer
 * to the LICENCE file in the root application directory.
 *
 * Version: 	1.0
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <QtGui/QApplication>
#include <QtGui/QLineEdit>

#include "application/MainWnd.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core  {

namespace test {

    using evias::application::MainWnd;

    class mainWnd_Full : public unitTest
    {
        public :

            mainWnd_Full () : unitTest () { };

            ~mainWnd_Full ();

            inline void setCall (int argc, char** argv)
                { _argc = argc; _argv = argv; _hasSetCall = true; }

            inline int prepare (vector<string> params)
            {
                return 2;
            }

            inline int execute ()
            {
                if (_hasSetCall != true) {
                    cout    << endl
                            << "-- setArgc & setArgv must be invoked before execute"
                            << endl << endl;

                    return -2;
                }

                string stylesheet = "       \
 QLineEdit {                                \
     border: 2px solid gray;                \
     border-radius: 10px;                   \
     padding: 0px 8px;                      \
     background: yellow;                    \
     selection-background-color: darkgray;  \
 }                                          \
";

                _qtApp = new QApplication(_argc, _argv);

                _wndObj = new MainWnd ();
                _wndObj->setWindowTitle ("evias mainWnd_Full unitary test");

                QLineEdit* editor = new QLineEdit (_wndObj);

                _wndObj->setStyleSheet (stylesheet.c_str ());

                _wndObj->show ();

                return _qtApp->exec();
            }

            inline int shutdown ()
            {
                cout << endl
                     << "-- Qt mainWnd_Full unit test shutdown"
                     << endl
                     << endl;

                if (_wndObj != NULL)
                    delete _wndObj;

                if (_qtApp != NULL)
                    delete _qtApp;

                return 0;
            }

        private :

            MainWnd*    _wndObj;
            QApplication* _qtApp;

            int         _argc;
            char**      _argv;
            bool        _hasSetCall;
    };

}; // end namespace test

}; // end namespace core

}; // end namespace evias

int main (int argc, char* argv[])
{
    using namespace std;

    evias::core::test::mainWnd_Full* unitTester = new evias::core::test::mainWnd_Full ();

    vector<string> params;

    params.push_back ("not used");

    int endCode = unitTester->prepare (params);

    if (endCode == 2) {
        unitTester->setCall (argc, argv);

        unitTester->execute ();

        endCode = unitTester->shutdown ();
    }

    return endCode;
}
