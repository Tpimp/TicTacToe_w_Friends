#-------------------------------------------------
#
# Project created by QtCreator 2015-05-11T21:10:53
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TicTacToeServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    tictactoeserver.cpp \
    listeningthread.cpp \
    tictactoegamethread.cpp \
    lobbypollthread.cpp \
    tictactoegame.cpp

HEADERS += \
    tictactoeserver.h \
    listeningthread.h \
    tictactoegamethread.h \
    lobbypollthread.h \
    tictactoegame.h

CONFIG += C++11
