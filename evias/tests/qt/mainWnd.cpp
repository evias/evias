#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <QtGui/QApplication>

#include "application/MainWnd.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core  {

namespace test {

    using evias::application::MainWnd;

    class mainWnd : public unitTest
    {
        public :

            mainWnd () : unitTest () { };

            ~mainWnd ();

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

                _qtApp = new QApplication(_argc, _argv);

                _wndObj = new MainWnd ();
                _wndObj->setWindowTitle ("evias mainWnd unitary test");

                _wndObj->show ();

                return _qtApp->exec();
            }

            inline int shutdown ()
            {
                cout << endl
                     << "-- Qt mainWnd unit test shutdown"
                     << endl
                     << endl;

                if (_wndObj != NULL)
                    delete _wndObj;

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

    evias::core::test::mainWnd* unitTester = new evias::core::test::mainWnd ();

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
