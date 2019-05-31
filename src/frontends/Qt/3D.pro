#-------------------------------------------------
#
# Project created by QtCreator 2018-09-26T20:39:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3D
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
        canvas.cpp \
        src/renderengine.cpp \
	../../engine/point3d.cpp \	
	../../engine/world.cpp \
	../../engine/camera.cpp \
	../../engine/rasteriser.cpp \
	../../engine/point2d.cpp \
	../../engine/triangle.cpp \
	../../engine/matrix.cpp

HEADERS += \
        src/mainwindow.h \
        canvas.h \
        src/renderengine.h \
	../../engine/point3d.h \
	../../engine/world.h \
	../../engine/camera.h \
	../../engine/rasteriser.h \
	../../engine/point2d.h \
	../../engine/triangle.h \
	../../engine/matrix.h

FORMS += \
        mainwindow.ui