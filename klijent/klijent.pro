#-------------------------------------------------
#
# Project created by QtCreator 2018-12-05T15:50:20
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = klijent
TEMPLATE = app
CONFIG += c++11

DEFINES += CLIENT

SOURCES += main.cpp\
        mainwindow.cpp \
    ../shared/client.cpp \
    ../shared/packet/client/ccontestapply.cpp \
    ../shared/packet/client/ckeepalive.cpp \
    ../shared/packet/client/cleavecontest.cpp \
    ../shared/packet/client/clientpacket.cpp \
    ../shared/packet/client/clogin.cpp \
    ../shared/packet/client/cproblemsolution.cpp \
    ../shared/packet/server/scontestapplyre.cpp \
    ../shared/packet/server/serverpacket.cpp \
    ../shared/packet/server/skeepalive.cpp \
    ../shared/packet/server/sleavere.cpp \
    ../shared/packet/server/sloginre.cpp \
    ../shared/packet/packet.cpp \
    ../shared/datastream.cpp \
    ../shared/problementry.cpp \
    user.cpp \
    ../shared/packet/server/scontestinfo.cpp \
    ../shared/packet/server/sproblemset.cpp \
    ../shared/datahandler.cpp \
    ../shared/packet/server/sproblemsolutionstatus.cpp \
    contest.cpp \
    ../shared/packet/server/supdateuserscore.cpp \
    ../shared/packet/server/suserleavecontest.cpp \
    ../shared/packet/server/scontestfinished.cpp \
    popuplogin.cpp \
    popupchoice.cpp

HEADERS  += mainwindow.h \
    ../shared/client.h \
    ../shared/packet/client/ccontestapply.h \
    ../shared/packet/client/ckeepalive.h \
    ../shared/packet/client/cleavecontest.h \
    ../shared/packet/client/clientpacket.h \
    ../shared/packet/client/clogin.h \
    ../shared/packet/client/cproblemsolution.h \
    ../shared/packet/server/scontestapplyre.h \
    ../shared/packet/server/serverpacket.h \
    ../shared/packet/server/skeepalive.h \
    ../shared/packet/server/sleavere.h \
    ../shared/packet/server/sloginre.h \
    ../shared/packet/packet.h \
    ../shared/datastream.h \
    ../shared/problementry.h \
    user.h \
    ../shared/packet/server/scontestinfo.h \
    ../shared/packet/server/sproblemset.h \
    ../shared/datahandler.h \
    ../shared/packet/server/sproblemsolutionstatus.h \
    contest.h \
    ../shared/packet/server/supdateuserscore.h \
    ../shared/packet/server/suserleavecontest.h \
    ../shared/packet/server/scontestfinished.h \
    popuplogin.h \
    popupchoice.h

FORMS    += mainwindow.ui \
    popuplogin.ui \
    popupchoice.ui

DISTFILES += \
    PopupLoginForm.ui.qml \
    PopupLogin.qml
