#-------------------------------------------------
#
# Project created by QtCreator 2015-04-29T17:48:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    controller.cpp \
    histogram.cpp

HEADERS  += mainwindow.h \
    controller.h \
    histogram.h

FORMS    += mainwindow.ui

LIBS += `pkg-config opencv --libs`

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
