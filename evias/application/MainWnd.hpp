#ifndef __EVIAS_ISCHOOL_MAINWND__
#define __EVIAS_ISCHOOL_MAINWND__

#include <QtGui/QMainWindow>
#include <QtGui/QWidget>

#include <string>

namespace evias {

namespace application {

    using namespace std;

    class MainWnd : public QMainWindow
    {
        Q_OBJECT

        public :

            MainWnd (QWidget* parent = 0, Qt::WindowFlags fl = 0);

            ~MainWnd ();

        private :

            string  _currentTitle;

    }; // end class MainWnd

}; // end namespace application

}; // end namespace evias

#endif //__EVIAS_ISCHOOL_MAINWND__

