QT += core
QT -= gui

TARGET = TestsFor_BeamSplitting
CONFIG += console
CONFIG += SSE
CONFIG += __SSE__
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

#QMAKE_CFLAGS_RELEASE-=-O2
QMAKE_CFLAGS-=-O2
QMAKE_CFLAGS+=-march=native -msse4.2 -O0
#QMAKE_CXXFLAGS="${CFLAGS}"
#QMAKE_CXXFLAGS_RELEASE-=-O2
QMAKE_CXXFLAGS-=-O2
QMAKE_CXXFLAGS+=-march=native -msse4.2 -O0
HEADERS += \
    intrinsics.h
