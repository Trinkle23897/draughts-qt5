#-------------------------------------------------
#
# Project created by QtCreator 2017-09-05T19:49:31
#
#-------------------------------------------------

QT += core gui
QT += multimedia
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = draughts
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mat.cpp \
    solver.cpp \
    enterhost.cpp

HEADERS  += mainwindow.h \
    mat.h \
    solver.h \
    enterhost.h

FORMS    += mainwindow.ui \
    enterhost.ui

RESOURCES += \
    res.qrc
