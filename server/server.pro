QT += core
QT -= gui
QT += network



TARGET = server
CONFIG += c++11 console
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    contestant.cpp

HEADERS += \
    server.h \
    contestant.h

