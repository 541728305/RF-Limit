/****************************************************************************
** Meta object code from reading C++ file 'multicurvesplot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../multicurvesplot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multicurvesplot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_my_Thread_t {
    QByteArrayData data[6];
    char stringdata0[50];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_my_Thread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_my_Thread_t qt_meta_stringdata_my_Thread = {
    {
QT_MOC_LITERAL(0, 0, 9), // "my_Thread"
QT_MOC_LITERAL(1, 10, 20), // "showDataSeriesSignal"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 5), // "count"
QT_MOC_LITERAL(4, 38, 6), // "finish"
QT_MOC_LITERAL(5, 45, 4) // "show"

    },
    "my_Thread\0showDataSeriesSignal\0\0count\0"
    "finish\0show"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_my_Thread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    0,   32,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void my_Thread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<my_Thread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showDataSeriesSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->finish(); break;
        case 2: _t->show((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (my_Thread::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&my_Thread::showDataSeriesSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (my_Thread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&my_Thread::finish)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject my_Thread::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_my_Thread.data,
    qt_meta_data_my_Thread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *my_Thread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *my_Thread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_my_Thread.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int my_Thread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void my_Thread::showDataSeriesSignal(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void my_Thread::finish()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_MultiCurvesPlot_t {
    QByteArrayData data[37];
    char stringdata0[413];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultiCurvesPlot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultiCurvesPlot_t qt_meta_stringdata_MultiCurvesPlot = {
    {
QT_MOC_LITERAL(0, 0, 15), // "MultiCurvesPlot"
QT_MOC_LITERAL(1, 16, 18), // "noniusValueChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 9), // "_curveIdx"
QT_MOC_LITERAL(4, 46, 3), // "val"
QT_MOC_LITERAL(5, 50, 11), // "coordChange"
QT_MOC_LITERAL(6, 62, 12), // "setCurveName"
QT_MOC_LITERAL(7, 75, 3), // "idx"
QT_MOC_LITERAL(8, 79, 7), // "newName"
QT_MOC_LITERAL(9, 87, 10), // "showCurves"
QT_MOC_LITERAL(10, 98, 15), // "QList<uint16_t>"
QT_MOC_LITERAL(11, 114, 8), // "_idxList"
QT_MOC_LITERAL(12, 123, 5), // "start"
QT_MOC_LITERAL(13, 129, 4), // "stop"
QT_MOC_LITERAL(14, 134, 11), // "startOffset"
QT_MOC_LITERAL(15, 146, 7), // "isLimit"
QT_MOC_LITERAL(16, 154, 14), // "storeAxisScope"
QT_MOC_LITERAL(17, 169, 1), // "x"
QT_MOC_LITERAL(18, 171, 1), // "y"
QT_MOC_LITERAL(19, 173, 15), // "resumeAxisScope"
QT_MOC_LITERAL(20, 189, 14), // "calloutSeries1"
QT_MOC_LITERAL(21, 204, 16), // "QCPDataSelection"
QT_MOC_LITERAL(22, 221, 6), // "select"
QT_MOC_LITERAL(23, 228, 14), // "calloutSeries2"
QT_MOC_LITERAL(24, 243, 15), // "showLimitSeries"
QT_MOC_LITERAL(25, 259, 7), // "upLimit"
QT_MOC_LITERAL(26, 267, 9), // "downLimit"
QT_MOC_LITERAL(27, 277, 12), // "clearAllData"
QT_MOC_LITERAL(28, 290, 27), // "when_selectionChangedByUser"
QT_MOC_LITERAL(29, 318, 22), // "when_legendDoubleClick"
QT_MOC_LITERAL(30, 341, 10), // "QCPLegend*"
QT_MOC_LITERAL(31, 352, 6), // "legend"
QT_MOC_LITERAL(32, 359, 22), // "QCPAbstractLegendItem*"
QT_MOC_LITERAL(33, 382, 4), // "item"
QT_MOC_LITERAL(34, 387, 12), // "QMouseEvent*"
QT_MOC_LITERAL(35, 400, 5), // "event"
QT_MOC_LITERAL(36, 406, 6) // "finish"

    },
    "MultiCurvesPlot\0noniusValueChanged\0\0"
    "_curveIdx\0val\0coordChange\0setCurveName\0"
    "idx\0newName\0showCurves\0QList<uint16_t>\0"
    "_idxList\0start\0stop\0startOffset\0isLimit\0"
    "storeAxisScope\0x\0y\0resumeAxisScope\0"
    "calloutSeries1\0QCPDataSelection\0select\0"
    "calloutSeries2\0showLimitSeries\0upLimit\0"
    "downLimit\0clearAllData\0"
    "when_selectionChangedByUser\0"
    "when_legendDoubleClick\0QCPLegend*\0"
    "legend\0QCPAbstractLegendItem*\0item\0"
    "QMouseEvent*\0event\0finish"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultiCurvesPlot[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   79,    2, 0x06 /* Public */,
       5,    0,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   85,    2, 0x0a /* Public */,
       9,    5,   90,    2, 0x0a /* Public */,
      16,    2,  101,    2, 0x0a /* Public */,
      19,    2,  106,    2, 0x0a /* Public */,
      20,    1,  111,    2, 0x0a /* Public */,
      23,    1,  114,    2, 0x0a /* Public */,
      24,    5,  117,    2, 0x0a /* Public */,
      27,    0,  128,    2, 0x08 /* Private */,
      28,    0,  129,    2, 0x08 /* Private */,
      29,    3,  130,    2, 0x08 /* Private */,
      36,    0,  137,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    7,    8,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,   17,   18,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,   17,   18,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   12,   13,   14,   25,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 30, 0x80000000 | 32, 0x80000000 | 34,   31,   33,   35,
    QMetaType::Void,

       0        // eod
};

void MultiCurvesPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MultiCurvesPlot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->noniusValueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->coordChange(); break;
        case 2: _t->setCurveName((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->showCurves((*reinterpret_cast< QList<uint16_t>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 4: _t->storeAxisScope((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->resumeAxisScope((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 6: _t->calloutSeries1((*reinterpret_cast< QCPDataSelection(*)>(_a[1]))); break;
        case 7: _t->calloutSeries2((*reinterpret_cast< QCPDataSelection(*)>(_a[1]))); break;
        case 8: _t->showLimitSeries((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 9: _t->clearAllData(); break;
        case 10: _t->when_selectionChangedByUser(); break;
        case 11: _t->when_legendDoubleClick((*reinterpret_cast< QCPLegend*(*)>(_a[1])),(*reinterpret_cast< QCPAbstractLegendItem*(*)>(_a[2])),(*reinterpret_cast< QMouseEvent*(*)>(_a[3]))); break;
        case 12: _t->finish(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPDataSelection >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPDataSelection >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPAbstractLegendItem* >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPLegend* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MultiCurvesPlot::*)(int , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MultiCurvesPlot::noniusValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MultiCurvesPlot::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MultiCurvesPlot::coordChange)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MultiCurvesPlot::staticMetaObject = { {
    QMetaObject::SuperData::link<QCustomPlot::staticMetaObject>(),
    qt_meta_stringdata_MultiCurvesPlot.data,
    qt_meta_data_MultiCurvesPlot,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MultiCurvesPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultiCurvesPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MultiCurvesPlot.stringdata0))
        return static_cast<void*>(this);
    return QCustomPlot::qt_metacast(_clname);
}

int MultiCurvesPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCustomPlot::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void MultiCurvesPlot::noniusValueChanged(int _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MultiCurvesPlot::coordChange()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
