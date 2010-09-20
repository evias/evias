#ifndef __EVIAS_APPLICATION_VIEW__
#define __EVIAS_APPLICATION_VIEW__

#include <QtGui/QWidget>

#include "MainWnd.hpp"

namespace evias {

namespace application {

    class basicView : public QWidget {
        Q_OBJECT

        public :

            basicView (QWidget* parent = 0, Qt::WindowFlags fl = 0);

            ~basicView ();

            virtual void render () = 0;

    };

};

};

#endif
