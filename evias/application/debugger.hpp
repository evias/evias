#ifndef __EVIAS_ISCHOOL_DEBUGGER__
#define __EVIAS_ISCHOOL_DEBUGGER__

// STL
#include <fstream>
#include <string>
#include <vector>
#include <map>

// QtGui widgets
#include <QtGui/QWidget>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QTextEdit>
#include <QtGui/QTabWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QFileDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>

// QtGui layouts
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>

// QtCore
#include <QtCore/QFile>
#include <QtCore/QTextStream>

// internal
#include "../core/date.hpp"
#include "MainWnd.hpp"
#include "TailEditor.hpp"

namespace evias {

namespace application {

    using namespace std;

    using evias::core::Date;
    using evias::application::TailEditor;

    class Debugger : public MainWnd
    {
        Q_OBJECT

        public :
            Debugger ();

            ~Debugger ();

            void log (string);

        public slots :

            QWidget*    addLogsTab ();

        private :

        // private member functions

            void        _initMenu  ();
            void        _initLayout();

            QToolBar*   _initLogsToolbar ();

        // private member variables

            string      _logFilePath;

            QMenu*      _mainMenu;
            QMenu*      _actionsMenu;
            QMenu*      _viewMenu;
            QMenu*      _aboutMenu;

            QTabWidget* _logTabWidget;

            QToolBar*   _logsToolBar;

            vector<string>  _logsOpened;

            vector<QWidget*> _tabsOpened;
    };

}; // end namespace application

}; // end namespace evias

#endif
