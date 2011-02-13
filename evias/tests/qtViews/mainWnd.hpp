#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <QtGui/QApplication>

#include "../../application/MainWnd.hpp"
#include "../../core/unit_test_abstract.hpp"

namespace evias {

namespace core  {

namespace test {

namespace qtViews {

    using evias::application::MainWnd;

    class mainWnd : public unitTest
    {
        public :

            mainWnd () : unitTest () { };

            ~mainWnd ();

            // XXX memcpy argv
            inline void setCall(int argc, char** argv)
                { _argc = argc; _argv = argv; };

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                _qtApp = new QApplication(_argc, _argv);

                _wndObj = new MainWnd ();
                _wndObj->setWindowTitle ("evias mainWnd unitary test");

                _wndObj->show ();

                return setReturnCode((_qtApp->exec() == 0) ? (int) RETURN_SUCCESS : (int) ERROR_ENVIRONMENT);
            }

            inline int shutdown ()
            {
                if (_wndObj != NULL)
                    delete _wndObj;

                return _returnCode;
            }

        private :

            MainWnd*    _wndObj;
            QApplication* _qtApp;

            int         _argc;
            char**      _argv;
    };

}; // end namespace qtViews

}; // end namespace test

}; // end namespace core

}; // end namespace evias

