#-------------------------------------------------
#
# Project created by QtCreator 2017-02-20T13:39:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = LabControle1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    funcoes.cpp \
    qcustomplot.cpp \
    pid.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    funcoes.h \
    quanser.h \
    pid.h

FORMS    += mainwindow.ui
