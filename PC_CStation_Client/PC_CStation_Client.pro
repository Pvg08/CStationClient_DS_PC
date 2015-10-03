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
    LIBS += $$PWD/libs/winapi/User32.Lib
    LIBS += $$PWD/libs/fmod/windows/lib/fmod_vc.lib
    INCLUDEPATH += $$PWD/libs/fmod/windows/inc
    DEPENDPATH += $$PWD/libs/fmod/windows/inc
    PRE_TARGETDEPS += $$PWD/libs/fmod/windows/lib/fmod.dll
    include(deployment.pri)
} else:win64 {
    LIBS += $$PWD/libs/winapi/User32.Lib
    LIBS += $$PWD/libs/fmod/windows/lib/fmod64_vc.lib
    INCLUDEPATH += $$PWD/libs/fmod/windows/inc
    DEPENDPATH += $$PWD/libs/fmod/windows/inc
    PRE_TARGETDEPS += $$PWD/libs/fmod/windows/lib/fmod64.dll
    include(deployment.pri)
} else:unix {
    *-64 {
        LIBS += -L$$PWD/libs/fmod/linux/lib/x86_64/ -lfmod
        PRE_TARGETDEPS += $$PWD/api/linux/lib/x86_64/libfmod.so
    } else {
        LIBS += -L$$PWD/libs/fmod/linux/lib/x86/ -lfmod
        PRE_TARGETDEPS += $$PWD/api/linux/lib/x86/libfmod.so
    }
    INCLUDEPATH += $$PWD/libs/fmod/linux/inc
    DEPENDPATH += $$PWD/libs/fmod/linux/inc
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
    classes/clientActions/clientactionindicationstate.cpp \
    classes/clientActions/clientactiontone.cpp \
    classes/clientActions/clientactionlcd.cpp

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
    classes/clientActions/clientactionindicationstate.h \
    classes/clientActions/clientactiontone.h \
    classes/clientActions/clientactionlcd.h

FORMS    += mainwindow.ui \
    widgets/clientitemlistsselector.ui

TRANSLATIONS += translations/PC_CStation_Client_ru_RU.ts

DISTFILES += \
    deployment.pri
