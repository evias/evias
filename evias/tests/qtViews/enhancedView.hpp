#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <QtGui/QApplication>
#include <QtGui/QLineEdit>

#include "../../application/MainWnd.hpp"
#include "core/unit_test_abstract.hpp"

namespace evias {

namespace core  {

namespace test {

namespace qtViews {

    using evias::application::MainWnd;

    class enhancedView : public unitTest
    {
        public :

            enhancedView () : unitTest () { };

            ~enhancedView () {};

            inline void setCall (int argc, char** argv)
                { _argc = argc; _argv = argv; _hasSetCall = true; }

            inline void prepare ()
            {
                setReturnCode((int) RETURN_SUCCESS);
            }

            inline int execute ()
            {
                string stylesheet = "       \
 QLineEdit {                                \
     border: 2px solid gray;                \
     border-radius: 10px;                   \
     padding: 0px 8px;                      \
     background: darkblue;                  \
     selection-background-color: darkgray;  \
     color: white;                          \
 }                                          \
";

                _qtApp = new QApplication(_argc, _argv);

                _wndObj = new MainWnd ();
                _wndObj->setWindowTitle ("evias enhancedView unitary test");

                QLineEdit* editor = new QLineEdit (_wndObj);

                _wndObj->setStyleSheet (stylesheet.c_str ());

                _wndObj->show ();

                return setReturnCode( (_qtApp->exec() == 0 ? (int) RETURN_SUCCESS : (int) ERROR_ENVIRONMENT) );
            }

            inline int shutdown ()
            {
                if (_wndObj != NULL)
                    delete _wndObj;

                if (_qtApp != NULL)
                    delete _qtApp;

                return _returnCode;
            }

        private :

            MainWnd*    _wndObj;
            QApplication* _qtApp;

            int         _argc;
            char**      _argv;
            bool        _hasSetCall;
    };

}; // end namespace qtViews

}; // end namespace test

}; // end namespace core

}; // end namespace evias

