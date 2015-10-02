#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T18:00:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PC_CStation_Client
TEMPLATE = app

win32 {
    LIBS += $$PWD/libs/User32.Lib
}

SOURCES += main.cpp\
    mainwindow.cpp \
    abstractserver.cpp \
    server.cpp \
    classes/clientsensor.cpp \
    classes/clientaction.cpp \
    widgets/clientitemlistsselector.cpp \
    classes/clientitem.cpp \
    classes/clientSensors/clientsensoractivity.cpp \
    classes/clientActions/clientactionreset.cpp \
    classes/clientActions/clientactionconfig.cpp \
    classes/clientSensors/clientsensorbtnactivity.cpp \
    classes/clientActions/clientactionindication.cpp \
    classes/clientActions/clientactionsetdisplaystate.cpp \
    classes/clientActions/clientactionindicationstate.cpp

HEADERS  += mainwindow.h \
    abstractserver.h \
    server.h \
    classes/clientsensor.h \
    classes/clientaction.h \
    widgets/clientitemlistsselector.h \
    classes/clientitem.h \
    classes/clientSensors/clientsensoractivity.h \
    classes/clientActions/clientactionreset.h \
    classes/clientActions/clientactionconfig.h \
    classes/clientSensors/clientsensorbtnactivity.h \
    classes/clientActions/clientactionindication.h \
    classes/clientActions/clientactionsetdisplaystate.h \
    classes/clientActions/clientactionindicationstate.h

FORMS    += mainwindow.ui \
    widgets/clientitemlistsselector.ui

TRANSLATIONS += translations/PC_CStation_Client_ru_RU.ts
