#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T18:44:47
#
#-------------------------------------------------

QT       += core gui opengl

CONFIG += c++11

INCLUDEPATH += C:\glm

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BezierSurface
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    canvaswidget.cpp \
    bezier.cpp \
    surface.cpp \
    heightmap.cpp

HEADERS  += mainwindow.h \
    canvaswidget.h \
    bezier.h \
    surface.h \
    stdafx.h \
    heightmap.h

FORMS    += mainwindow.ui
