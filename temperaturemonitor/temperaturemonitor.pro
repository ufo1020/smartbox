#-------------------------------------------------
#
# Project created by QtCreator 2015-06-08T20:03:18
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = temperatureMonitor
CONFIG   += console debug_and_release
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    httpservice.cpp \
    maincontroller.cpp \
    tmp36driver.cpp \
    temperaturesensor.cpp \
    temperaturemonitor.cpp \
    powerswitchdriver.cpp

HEADERS += \
    httpservice.h \
    maincontroller.h \
    tmp36driver.h \
    temperaturesensor.h \
    temperaturemonitor.h \
    powerswitchdriver.h \
    utility.h

# path for qhttpserver
LIBS += -L$$OUT_PWD/../qhttpserver/lib

win32 {
    debug: LIBS += -lqhttpserverd
    else: LIBS += -lqhttpserver
} else {
    LIBS += -lqhttpserver
}

INCLUDEPATH += $$PWD/../qhttpserver/src
DEPENDPATH += $$PWD/../qhttpserver/src

CONFIG += c++11

#CFLAGS += -Wno-write-strings
CONFIG += warn_off

