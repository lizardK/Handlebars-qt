QT += testlib
QT -= gui
CONFIG -= app_bundle
TEMPLATE = app

DESTDIR = dist/tests
MOC_DIR = build
OBJECTS_DIR = build

INCLUDEPATH += $$PWD/../lib/src/

LIBS += -Ldist/lib -lHandlebars-qt

SOURCES += $$PWD/src/test_handlebars.cpp
HEADERS += $$PWD/src/test_handlebars.h
