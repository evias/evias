#include "MainWnd.hpp"

namespace evias {

namespace application {

    MainWnd::MainWnd (QWidget* parent, Qt::WindowFlags fl)
        : QMainWindow (parent, fl)
    {
        setWindowTitle ("iSchool");
    }

    MainWnd::~MainWnd ()
    {

    }

}; // end namespace application

}; // end namespace evias

