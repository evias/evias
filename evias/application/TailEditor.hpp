#ifndef __EVIAS_DEBUGGER_TailEditor__
#define __EVIAS_DEBUGGER_TailEditor__

#include <QtGui/QWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <sys/types.h>
#include <sys/stat.h>

#include "../core/date.hpp"

#include <string>

namespace evias {

namespace application {

    using namespace std;

    using evias::core::Date;

    class TailEditor : public QWidget
    {
        Q_OBJECT

        public :

            TailEditor (QWidget* parent = 0);

            ~TailEditor ();

            inline bool hasData ()
                { return _hasData == true; }

            inline void setHasNoData (bool hasNoData = true)
                { _editor->setText ("No file opened"); _hasData = ! hasNoData; }

            inline void setFile (string file)
                { _tailedFilename = file; refreshEditor (); }

            inline QString lastData ()
                { return (*(_dataBackup.rbegin())).second; }

            inline QString currentData ()
                { return _content; }

        public slots :

            void refreshEditor ();
            void flushEditor   ();

        protected :

            void _parseFile ();

        private :

            string  _tailedFilename;
            bool    _hasData;

            QString _content;

            Date*   _lastModified;

            // GUI variables
            QPushButton*    _refreshButton;
            QPushButton*    _flushButton;

            QTextEdit*      _editor;

            map<Date*,QString>  _dataBackup;
    };

}; // application

}; // evias

#endif
