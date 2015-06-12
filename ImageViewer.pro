#-------------------------------------------------
#
# Project created by QtCreator 2015-05-11T17:06:02
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imageviewer.cpp

HEADERS  += mainwindow.h \
    imageviewer.h

FORMS    += mainwindow.ui

OTHER_FILES += /home/diman/.codestyle.astylerc
