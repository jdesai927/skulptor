/****************************************************************************
** Meta object code from reading C++ file 'my3dviewer.h'
**
** Created: Thu Apr 26 06:38:39 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../my3dviewer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'my3dviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_My3DViewer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      30,   11,   11,   11, 0x0a,
      43,   11,   11,   11, 0x0a,
      55,   11,   11,   11, 0x0a,
      65,   11,   11,   11, 0x0a,
      81,   11,   11,   11, 0x0a,
      91,   11,   11,   11, 0x0a,
     106,   11,   11,   11, 0x0a,
     123,   11,   11,   11, 0x0a,
     140,   11,   11,   11, 0x0a,
     159,  157,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_My3DViewer[] = {
    "My3DViewer\0\0changeFaceColor()\0"
    "deleteVert()\0splitFace()\0addVert()\0"
    "changeVertPos()\0edgeAdd()\0showHelpInfo()\0"
    "changeFaceR(int)\0changeFaceG(int)\0"
    "changeFaceB(int)\0f\0setColorSliders(Face*)\0"
};

void My3DViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        My3DViewer *_t = static_cast<My3DViewer *>(_o);
        switch (_id) {
        case 0: _t->changeFaceColor(); break;
        case 1: _t->deleteVert(); break;
        case 2: _t->splitFace(); break;
        case 3: _t->addVert(); break;
        case 4: _t->changeVertPos(); break;
        case 5: _t->edgeAdd(); break;
        case 6: _t->showHelpInfo(); break;
        case 7: _t->changeFaceR((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->changeFaceG((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->changeFaceB((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setColorSliders((*reinterpret_cast< Face*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData My3DViewer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject My3DViewer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_My3DViewer,
      qt_meta_data_My3DViewer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &My3DViewer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *My3DViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *My3DViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_My3DViewer))
        return static_cast<void*>(const_cast< My3DViewer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int My3DViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
