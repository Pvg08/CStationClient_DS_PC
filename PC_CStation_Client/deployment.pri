
CONFIG( release, debug|release ) {
    isEmpty(TARGET_EXT) {
        win32 {
            TARGET_CUSTOM_EXT = .exe
        }
    } else {
        TARGET_CUSTOM_EXT = $${TARGET_EXT}
    }
    win32 {
        DEPLOY_COMMAND = windeployqt
    }
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_CUSTOM_EXT}))
    warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})
    QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}
    DTYPE = release
} else:CONFIG(debug, debug|release) {
    DTYPE = debug
}

win32 {
    PR_LIB_PATH = $$PWD/libs/fmod/windows/lib
    PR_LIB_FILE = fmod.dll
} else:win64 {
    PR_LIB_PATH = $$PWD/libs/fmod/windows/lib
    PR_LIB_FILE = fmod64.dll
} else:unix {
    *-64 {
        PR_LIB_PATH = $$PWD/libs/fmod/linux/lib/x86_64
        PR_LIB_FILE = libfmod.so
    } else {
        PR_LIB_PATH = $$PWD/libs/fmod/linux/lib/x86
        PR_LIB_FILE = libfmod.so
    }
}

RR_LIB_FILE.commands = copy /Y $$shell_path($${PR_LIB_PATH}/$${PR_LIB_FILE}) $${DTYPE}
RR_LIB_FILE.target = $${DTYPE}/$${PR_LIB_FILE}
QMAKE_EXTRA_TARGETS += RR_LIB_FILE
PRE_TARGETDEPS += $${DTYPE}/$${PR_LIB_FILE}
