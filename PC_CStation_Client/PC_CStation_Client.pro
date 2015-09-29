#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T18:00:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PC_CStation_Client
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    abstractserver.cpp \
    server.cpp \
    classes/clientsensor.cpp \
    classes/clientaction.cpp \
    classes/clientSensors/clientsensoractivity.cpp \
    classes/clientActions/clientactionreset.cpp \
    classes/clientActions/clientactionconfig.cpp \
    classes/clientSensors/clientsensorbtnactivity.cpp

HEADERS  += mainwindow.h \
    abstractserver.h \
    server.h \
    classes/clientsensor.h \
    classes/clientaction.h \
    classes/clientSensors/clientsensoractivity.h \
    classes/clientActions/clientactionreset.h \
    classes/clientActions/clientactionconfig.h \
    classes/clientSensors/clientsensorbtnactivity.h

FORMS    += mainwindow.ui

TRANSLATIONS += translations/PC_CStation_Client_ru_RU.ts
