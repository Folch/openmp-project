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
    histogram.cpp \
    quicksort.cpp

HEADERS  += mainwindow.h \
    controller.h \
    histogram.h \
    quicksort.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -fopenmp -mavx -fabi-version=0 -ffast-math
QMAKE_LFLAGS += -fopenmp
QMAKE_CXXFLAGS_RELEASE *= -O3
QMAKE_CFLAGS_RELEASE += -fopenmp

LIBS += `pkg-config opencv --libs`
LIBS += -fopenmp
