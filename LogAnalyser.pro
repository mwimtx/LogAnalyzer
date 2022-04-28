#-------------------------------------------------
#
# Project created by QtCreator 2015-11-13T13:58:30
#
#-------------------------------------------------

QT       += core gui sql xml printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LogAnalyser
TEMPLATE = app


SOURCES += main.cpp         \
           mainwindow.cpp   \
           qcustomplot.cpp  \
           dbinterface.cpp  \
           xmlparses.cpp

HEADERS  += mainwindow.h    \
            qcustomplot.h   \
            dbinterface.h   \
            xmlparses.h

FORMS    += mainwindow.ui
