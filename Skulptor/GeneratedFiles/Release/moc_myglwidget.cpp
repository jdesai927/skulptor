/****************************************************************************
** Meta object code from reading C++ file 'myglwidget.h'
**
** Created: Thu Apr 26 19:43:48 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../myglwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myglwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyGLWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      39,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      35,   11,   11,   11, 0x0a,
      48,   11,   11,   11, 0x0a,
      61,   11,   11,   11, 0x0a,
      74,   11,   11,   11, 0x0a,
      86,   11,   11,   11, 0x0a,
     101,   11,   11,   11, 0x0a,
     119,   11,   11,   11, 0x0a,
     140,   11,   11,   11, 0x0a,
     155,   11,   11,   11, 0x0a,
     169,   11,   11,   11, 0x0a,
     184,   11,   11,   11, 0x0a,
     198,   11,   11,   11, 0x0a,
     214,   11,   11,   11, 0x0a,
     231,   11,   11,   11, 0x0a,
     253,   11,   11,   11, 0x0a,
     267,   11,   11,   11, 0x0a,
     284,   11,   11,   11, 0x0a,
     296,   11,   11,   11, 0x0a,
     308,   11,   11,   11, 0x0a,
     320,   11,   11,   11, 0x0a,
     335,   11,   11,   11, 0x0a,
     350,   11,   11,   11, 0x0a,
     365,   11,   11,   11, 0x0a,
     382,   11,   11,   11, 0x0a,
     401,   11,   11,   11, 0x0a,
     413,   11,   11,   11, 0x0a,
     429,   11,   11,   11, 0x0a,
     445,   11,   11,   11, 0x0a,
     459,   11,   11,   11, 0x0a,
     473,   11,   11,   11, 0x0a,
     489,   11,   11,   11, 0x0a,
     505,   11,   11,   11, 0x0a,
     521,   11,   11,   11, 0x0a,
     543,   11,   11,   11, 0x0a,
     551,   11,   11,   11, 0x0a,
     571,   11,   11,   11, 0x0a,
     591,   11,   11,   11, 0x0a,
     611,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MyGLWidget[] = {
    "MyGLWidget\0\0sendcurrentface(Face*)\0"
    "setrefx(int)\0setrefy(int)\0setrefz(int)\0"
    "setnewref()\0snaptoorigin()\0setoverheadview()\0"
    "setperspectiveview()\0setfrontview()\0"
    "setbackview()\0setrightview()\0setleftview()\0"
    "setcustomview()\0savecustomview()\0"
    "setselectionmode(int)\0saveObjOver()\0"
    "loadShaderFile()\0twistX(int)\0twistY(int)\0"
    "twistZ(int)\0setlightx(int)\0setlighty(int)\0"
    "setlightz(int)\0setlightsource()\0"
    "snapViewSelected()\0subdivide()\0"
    "subdivideMesh()\0resetedgeline()\0"
    "loadObjFile()\0saveObjFile()\0setdeformi(int)\0"
    "setdeformj(int)\0setdeformk(int)\0"
    "setdeformdimensions()\0twist()\0"
    "selectcontrolx(int)\0selectcontroly(int)\0"
    "selectcontrolz(int)\0setcurrentcontrolvertex()\0"
};

void MyGLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MyGLWidget *_t = static_cast<MyGLWidget *>(_o);
        switch (_id) {
        case 0: _t->sendcurrentface((*reinterpret_cast< Face*(*)>(_a[1]))); break;
        case 1: _t->setrefx((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setrefy((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setrefz((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setnewref(); break;
        case 5: _t->snaptoorigin(); break;
        case 6: _t->setoverheadview(); break;
        case 7: _t->setperspectiveview(); break;
        case 8: _t->setfrontview(); break;
        case 9: _t->setbackview(); break;
        case 10: _t->setrightview(); break;
        case 11: _t->setleftview(); break;
        case 12: _t->setcustomview(); break;
        case 13: _t->savecustomview(); break;
        case 14: _t->setselectionmode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->saveObjOver(); break;
        case 16: _t->loadShaderFile(); break;
        case 17: _t->twistX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->twistY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->twistZ((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->setlightx((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->setlighty((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->setlightz((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->setlightsource(); break;
        case 24: _t->snapViewSelected(); break;
        case 25: _t->subdivide(); break;
        case 26: _t->subdivideMesh(); break;
        case 27: _t->resetedgeline(); break;
        case 28: _t->loadObjFile(); break;
        case 29: _t->saveObjFile(); break;
        case 30: _t->setdeformi((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->setdeformj((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->setdeformk((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->setdeformdimensions(); break;
        case 34: _t->twist(); break;
        case 35: _t->selectcontrolx((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->selectcontroly((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->selectcontrolz((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: _t->setcurrentcontrolvertex(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MyGLWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MyGLWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_MyGLWidget,
      qt_meta_data_MyGLWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyGLWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyGLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyGLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyGLWidget))
        return static_cast<void*>(const_cast< MyGLWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int MyGLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    }
    return _id;
}

// SIGNAL 0
void MyGLWidget::sendcurrentface(Face * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
