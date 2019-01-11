QT += core
QT -= gui
QT += network



TARGET = server
CONFIG += c++11 console
CONFIG += console
CONFIG -= app_bundle

DEFINES += SERVER

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    contestant.cpp \
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
    contest/problemsetmanager.cpp \
    ../shared/packet/server/scontestinfo.cpp \
    ../shared/client.cpp \
    contest/contest.cpp \
    contest/contestmanager.cpp \
    ../shared/packet/server/sproblemset.cpp \
    ../shared/datahandler.cpp \
    ../shared/packet/server/supdateuserscore.cpp \
    ../shared/packet/server/sproblemsolutionstatus.cpp \
    ../shared/packet/server/suserleavecontest.cpp \
    ../shared/packet/server/scontestfinished.cpp \
    contest/solutionmanager.cpp

HEADERS += \
    server.h \
    contestant.h \
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
    contest/problemsetmanager.h \
    ../shared/packet/server/scontestinfo.h \
    ../shared/client.h \
    contest/contest.h \
    contest/contestmanager.h \
    ../shared/packet/server/sproblemset.h \
    ../shared/datahandler.h \
    ../shared/packet/server/supdateuserscore.h \
    ../shared/packet/server/sproblemsolutionstatus.h \
    ../shared/packet/server/suserleavecontest.h \
    ../shared/packet/server/scontestfinished.h \
    contest/solutionmanager.h \
    contest/judge.h

