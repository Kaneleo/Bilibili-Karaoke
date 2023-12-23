/****************************************************************************
** Meta object code from reading C++ file 'myThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../personal/Bilibili-Karaoke/src/thread/myThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_myThread_t {
    QByteArrayData data[10];
    char stringdata0[92];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_myThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_myThread_t qt_meta_stringdata_myThread = {
    {
QT_MOC_LITERAL(0, 0, 8), // "myThread"
QT_MOC_LITERAL(1, 9, 14), // "download_start"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 24), // "sig_downloadCmd_finished"
QT_MOC_LITERAL(4, 50, 7), // "setFlag"
QT_MOC_LITERAL(5, 58, 4), // "flag"
QT_MOC_LITERAL(6, 63, 7), // "display"
QT_MOC_LITERAL(7, 71, 6), // "addurl"
QT_MOC_LITERAL(8, 78, 1), // "s"
QT_MOC_LITERAL(9, 80, 11) // "downloadCmd"

    },
    "myThread\0download_start\0\0"
    "sig_downloadCmd_finished\0setFlag\0flag\0"
    "display\0addurl\0s\0downloadCmd"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_myThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   51,    2, 0x0a /* Public */,
       4,    0,   54,    2, 0x2a /* Public | MethodCloned */,
       6,    0,   55,    2, 0x0a /* Public */,
       7,    1,   56,    2, 0x0a /* Public */,
       9,    0,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,

       0        // eod
};

void myThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<myThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->download_start(); break;
        case 1: _t->sig_downloadCmd_finished(); break;
        case 2: _t->setFlag((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setFlag(); break;
        case 4: _t->display(); break;
        case 5: _t->addurl((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->downloadCmd(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (myThread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&myThread::download_start)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (myThread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&myThread::sig_downloadCmd_finished)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject myThread::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_myThread.data,
    qt_meta_data_myThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *myThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *myThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_myThread.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int myThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void myThread::download_start()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void myThread::sig_downloadCmd_finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
