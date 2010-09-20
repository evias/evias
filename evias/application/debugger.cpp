#include "debugger.hpp"

namespace evias {

namespace application {

    Debugger::Debugger ()
        : MainWnd ()
    {
        _logFilePath = "/home/greg/srv/home.work/iSchool/resources/storage/debugger.log";

        setWindowTitle ("iSchool Debugger v1.0");

        resize (640, 480);

        _initMenu ();
        _initLayout ();

        log ("iSchool Debugger initialized.");
    }

    Debugger::~Debugger ()
    { }

    void Debugger::log (string msg)
    {
        ofstream ostr;
        ostr.open (_logFilePath.c_str(), ios::app);

        string time = Date::now()->toString();

        if (ostr.good())
            // append message
            ostr << time << " : " << msg << endl;

        ostr.close();
    }

    void Debugger::_initMenu ()
    {
        _mainMenu   = new QMenu ("Debugger");
        _actionsMenu= new QMenu ("Actions");
        _viewMenu   = new QMenu ("View");
        _aboutMenu  = new QMenu ("About");

        _mainMenu->addAction ("Quit", this, SLOT (close()));

        menuBar()->addMenu (_mainMenu);
        menuBar()->addMenu (_actionsMenu);
        menuBar()->addMenu (_viewMenu);
        menuBar()->addMenu (_aboutMenu);

        // set Debugger's menu bar
        setMenuBar (menuBar());
    }

    void Debugger::_initLayout ()
    {
        addToolBar (Qt::LeftToolBarArea, _initLogsToolbar ());

        _logTabWidget = new QTabWidget;

        _logTabWidget->addTab ( addLogsTab (), "Log Window 1" );

        // set Debugger's central widget
        setCentralWidget (_logTabWidget);
    }

    QToolBar* Debugger::_initLogsToolbar ()
    {
        _logsToolBar = new QToolBar ( "Logs" );

        _logsToolBar->addAction ("Add Log Window", this, SLOT (addLogsTab()));

        return _logsToolBar;
    }

    QWidget* Debugger::addLogsTab ()
    {
        QWidget* tab = new QWidget;
        QString fileName;

        int index = 0;

        if (! _tabsOpened.empty()) {
            index = _tabsOpened.size ();

            fileName = QFileDialog::getOpenFileName (this, tr ("Open log file"), "/", tr ("All files (*)"));

            if (fileName == "/") {
                // no file selected
                fileName = "";
            }

            index++;
        }
        else {
            fileName = "/home/greg/srv/home.work/iSchool/resources/storage/debugger.log";
        }

        string editorContent = "";

        vector<QString> lines;

        TailEditor* fileContentDisplay = new TailEditor;

        if (! fileName.isEmpty()) {
            fileContentDisplay->setFile (fileName.toStdString());
        }
        else {
            // no file opened, empty text edit
            fileContentDisplay->setHasNoData (true);
        }

        QLineEdit* fileNameDisplay = new QLineEdit;
        fileNameDisplay->setText (fileName);

        QVBoxLayout* boxLayout = new QVBoxLayout;

        boxLayout->addWidget (fileNameDisplay);
        boxLayout->addWidget (fileContentDisplay);

        tab->setLayout (boxLayout);

        string tabStyleSheet = "            \
 QLineEdit {                                \
     border: 2px solid gray;                \
     border-radius: 10px;                   \
     padding: 0px 25px;                     \
     background: green;                     \
     color: black;                          \
 }                                          \
";

        tab->setStyleSheet (tabStyleSheet.c_str());

        _tabsOpened.push_back (tab);
        if (! _tabsOpened.empty ()) {
            // needs to add the tab to the _logTabWidget
            _logTabWidget->addTab (tab, QString ("Log Window %1").arg (index));
        }

        return tab;
    }

}; // end namespace application

}; // end namespace evias

