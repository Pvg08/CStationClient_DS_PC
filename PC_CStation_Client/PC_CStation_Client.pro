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
    server.cpp

HEADERS  += mainwindow.h \
    server.h

FORMS    += mainwindow.ui

TRANSLATIONS += translations/PC_CStation_Client_ru_RU.ts
