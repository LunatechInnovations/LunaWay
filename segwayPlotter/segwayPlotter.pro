#-------------------------------------------------
#
# Project created by QtCreator 2014-01-12T02:03:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = segwayPlotter
TEMPLATE = app


SOURCES += main.cpp\
        segwayplotter.cpp \
    plotserver.cpp \
    qcustomplot.cpp

HEADERS  += segwayplotter.h \
    plotserver.h \
    qcustomplot.h

FORMS    += segwayplotter.ui
