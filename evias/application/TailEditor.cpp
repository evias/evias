#include "TailEditor.hpp"

namespace evias {

namespace application {

    TailEditor::TailEditor (QWidget* parent)
        : QWidget(parent)
    {
        QVBoxLayout* editorLayout = new QVBoxLayout;
        QHBoxLayout* actionsLayout= new QHBoxLayout;

        QGroupBox* actionsBox  = new QGroupBox ("Editor actions");

        _refreshButton = new QPushButton ("Refresh");
        _flushButton   = new QPushButton ("Flush");

        string buttonSS = "                 \
            border: 1px solid black;        \
            border-radius: 5px;             \
            padding: 5px 3px;               \
            font: bold 10px;                \
            height: 16px;                   \
        ";

        _refreshButton->setStyleSheet (buttonSS.c_str());
        _flushButton->setStyleSheet (buttonSS.c_str());

        _editor = new QTextEdit;

        _editor->setStyleSheet ("           \
            border : 2px solid white;       \
            border-radius: 10px;            \
            padding: 5px 10px;              \
            background: white;              \
            color: darkgreen;               \
        ");

        _editor->setReadOnly(true);

        actionsLayout->addWidget (_refreshButton);
        actionsLayout->addWidget (_flushButton);
        actionsBox->setLayout (actionsLayout);

        editorLayout->addWidget (actionsBox);
        editorLayout->addWidget (_editor);
        setLayout (editorLayout);

        connect (_refreshButton, SIGNAL (clicked()), this, SLOT (refreshEditor()));
        connect (_flushButton, SIGNAL (clicked()), this, SLOT (flushEditor()));
    }

    TailEditor::~TailEditor ()
    {
        if (_refreshButton != NULL)
            delete _refreshButton;

        if (_flushButton != NULL)
            delete _flushButton;

        if (_editor != NULL)
            delete _editor;
    }

    void TailEditor::refreshEditor ()
    {
        _parseFile ();

        _editor->setText (_content);
    }

    void TailEditor::flushEditor ()
    {
        // backup data
        _dataBackup.insert (make_pair (Date::now(), _content));

        _editor->clear();
    }

    void TailEditor::_parseFile ()
    {
        QFile file (_tailedFilename.c_str());

        if (file.open (QIODevice::ReadOnly)) {
            QTextStream stream (&file);

            _content = stream.readAll();
        }

        file.close();
    }

}; // application

}; // evias

