/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: sab 10. giu 11:58:50 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_MainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      27,   11,   11,   11, 0x0a,
      43,   38,   11,   11, 0x0a,
      63,   38,   11,   11, 0x0a,
      87,   85,   11,   11, 0x0a,
     112,  105,   11,   11, 0x0a,
     150,  105,   11,   11, 0x0a,
     180,   38,   11,   11, 0x0a,
     209,   38,   11,   11, 0x0a,
     234,   85,   11,   11, 0x0a,
     255,  105,   11,   11, 0x0a,
     296,  105,   11,   11, 0x0a,
     329,   11,   11,   11, 0x08,
     343,   11,   11,   11, 0x08,
     358,   11,   11,   11, 0x08,
     368,   11,   11,   11, 0x08,
     377,   11,   11,   11, 0x08,
     394,  388,   11,   11, 0x08,
     424,   11,   11,   11, 0x08,
     432,   11,   11,   11, 0x08,
     439,   11,   11,   11, 0x08,
     446,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0daAggiornare()\0aggiorna()\0info\0appendPCInfo(char*)\0"
    "appendPCInfo(QString)\0i\0appendPCInfo(int)\0info,i\0"
    "appendPCInfo(const char*,const char*)\0appendPCInfo(const char*,int)\0"
    "appendRobotInfo(const char*)\0appendRobotInfo(QString)\0"
    "appendRobotInfo(int)\0appendRobotInfo(const char*,const char*)\0"
    "appendRobotInfo(const char*,int)\0openComPort()\0closeComPort()\0"
    "portSel()\0setPos()\0robotSel()\0event\0mousePressEvent(QMouseEvent*)\0"
    "start()\0stop()\0goal()\0sendCmd()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
	return static_cast<void*>(const_cast<MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: daAggiornare(); break;
        case 1: aggiorna(); break;
        case 2: appendPCInfo(*reinterpret_cast< char*(*)>(_a[1])); break;
        case 3: appendPCInfo(*reinterpret_cast< QString(*)>(_a[1])); break;
        case 4: appendPCInfo(*reinterpret_cast< int(*)>(_a[1])); break;
        case 5: appendPCInfo(*reinterpret_cast< const char*(*)>(_a[1]),*reinterpret_cast< const char*(*)>(_a[2])); break;
        case 6: appendPCInfo(*reinterpret_cast< const char*(*)>(_a[1]),*reinterpret_cast< int(*)>(_a[2])); break;
        case 7: appendRobotInfo(*reinterpret_cast< const char*(*)>(_a[1])); break;
        case 8: appendRobotInfo(*reinterpret_cast< QString(*)>(_a[1])); break;
        case 9: appendRobotInfo(*reinterpret_cast< int(*)>(_a[1])); break;
        case 10: appendRobotInfo(*reinterpret_cast< const char*(*)>(_a[1]),*reinterpret_cast< const char*(*)>(_a[2])); break;
        case 11: appendRobotInfo(*reinterpret_cast< const char*(*)>(_a[1]),*reinterpret_cast< int(*)>(_a[2])); break;
        case 12: openComPort(); break;
        case 13: closeComPort(); break;
        case 14: portSel(); break;
        case 15: setPos(); break;
        case 16: robotSel(); break;
        case 17: mousePressEvent(*reinterpret_cast< QMouseEvent*(*)>(_a[1])); break;
        case 18: start(); break;
        case 19: stop(); break;
        case 20: goal(); break;
        case 21: sendCmd(); break;
        }
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::daAggiornare()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
