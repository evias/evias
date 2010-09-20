/****************************************************************************
** Meta object code from reading C++ file 'debugger.hpp'
**
** Created: Mon Sep 20 04:01:20 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "evias/application/debugger.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'debugger.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_evias__application__Debugger[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      39,   29,   30,   29, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_evias__application__Debugger[] = {
    "evias::application::Debugger\0\0QWidget*\0"
    "addLogsTab()\0"
};

const QMetaObject evias::application::Debugger::staticMetaObject = {
    { &MainWnd::staticMetaObject, qt_meta_stringdata_evias__application__Debugger,
      qt_meta_data_evias__application__Debugger, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &evias::application::Debugger::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *evias::application::Debugger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *evias::application::Debugger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_evias__application__Debugger))
        return static_cast<void*>(const_cast< Debugger*>(this));
    return MainWnd::qt_metacast(_clname);
}

int evias::application::Debugger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MainWnd::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QWidget* _r = addLogsTab();
            if (_a[0]) *reinterpret_cast< QWidget**>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
