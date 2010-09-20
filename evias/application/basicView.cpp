#include "basicView.hpp"

namespace evias {

namespace application {

    basicView::basicView (QWidget* parent, Qt::WindowFlags fl)
        : QWidget (parent, fl)
    {
        setWindowTitle ("iSchool Window");
    }

    basicView::~basicView ()
    {

    }

}; // application

}; // evias

