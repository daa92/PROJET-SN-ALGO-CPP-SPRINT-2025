/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "on_librarianRadioButton_toggled",
    "",
    "checked",
    "on_studentTeacherRadioButton_toggled",
    "on_loginButton_clicked",
    "on_addBookButton_clicked",
    "on_removeBookButton_clicked",
    "on_sendUpdatesButton_clicked",
    "on_borrowBookButton_clicked",
    "on_returnBookButton_clicked",
    "on_reserveBookButton_clicked",
    "on_cancelReservationButton_clicked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x08,    1 /* Private */,
       4,    1,   77,    2, 0x08,    3 /* Private */,
       5,    0,   80,    2, 0x08,    5 /* Private */,
       6,    0,   81,    2, 0x08,    6 /* Private */,
       7,    0,   82,    2, 0x08,    7 /* Private */,
       8,    0,   83,    2, 0x08,    8 /* Private */,
       9,    0,   84,    2, 0x08,    9 /* Private */,
      10,    0,   85,    2, 0x08,   10 /* Private */,
      11,    0,   86,    2, 0x08,   11 /* Private */,
      12,    0,   87,    2, 0x08,   12 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'on_librarianRadioButton_toggled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_studentTeacherRadioButton_toggled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_loginButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_addBookButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_removeBookButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_sendUpdatesButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_borrowBookButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_returnBookButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_reserveBookButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_cancelReservationButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_librarianRadioButton_toggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->on_studentTeacherRadioButton_toggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->on_loginButton_clicked(); break;
        case 3: _t->on_addBookButton_clicked(); break;
        case 4: _t->on_removeBookButton_clicked(); break;
        case 5: _t->on_sendUpdatesButton_clicked(); break;
        case 6: _t->on_borrowBookButton_clicked(); break;
        case 7: _t->on_returnBookButton_clicked(); break;
        case 8: _t->on_reserveBookButton_clicked(); break;
        case 9: _t->on_cancelReservationButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
