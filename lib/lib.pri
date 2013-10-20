QT       += qml
QT       -= gui

TARGET = Handlebars-qt
TEMPLATE = lib

DEFINES += HANDLEBARSQT_LIBRARY

DESTDIR = dist/lib
MOC_DIR = build
OBJECTS_DIR = build

SOURCES += $$PWD/src/handlebars.cpp

HEADERS += $$PWD/src/handlebars.h\
        $$PWD/src/handlebars-qt_global.h

RESOURCES += \
    lib/lib_resources.qrc
