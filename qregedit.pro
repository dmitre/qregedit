QT       += core gui widgets

TARGET = qregedit
TEMPLATE = app

CONFIG += warn_on link_pkgconfig c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    chntpw/ntreg.c \
    registrymodel.cpp \
    global.cpp \
    regutils.cpp \
    valueeditor.cpp \
    settingsdlg.cpp \
    functions.cpp \
    progressdialog.cpp \
    finder.cpp \
    logdisplay.cpp \
    chntpw/chntpw.c \
    chntpw/libsam.c \
    sammodel.cpp

HEADERS  += mainwindow.h \
    chntpw/ntreg.h \
    registrymodel.h \
    global.h \
    regutils.h \
    valueeditor.h \
    settingsdlg.h \
    functions.h \
    progressdialog.h \
    finder.h \
    logdisplay.h \
    chntpw/sam.h \
    chntpw/chntpw.h \
    sammodel.h

FORMS    += mainwindow.ui \
    valueeditor.ui \
    settingsdlg.ui \
    progressdialog.ui \
    logdisplay.ui

OTHER_FILES += \
    LICENSE \
    README.md

RESOURCES += \
    qregedit.qrc

CONFIG += lanman_crypto

lanman_crypto {
    DEFINES += DOCRYPTO=1
    PKGCONFIG += openssl
}

include( qhexedit2/qhexedit.pri )
