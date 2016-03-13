QT += core
QT -= gui

TARGET = TestsFor_BeamSplitting
CONFIG += console
CONFIG += SSE
CONFIG += __SSE__
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp


QMAKE_CFLAGS-=-O2
QMAKE_CFLAGS+=-march=corei7 -msse4.2 -O2

QMAKE_CXXFLAGS-=-O2
QMAKE_CXXFLAGS+=-march=corei7 -msse4.2 -O2
HEADERS += \
    intrinsics.h \
    types.h \
    non_intrinsics.h

OTHER_FILES += \
    log.txt
