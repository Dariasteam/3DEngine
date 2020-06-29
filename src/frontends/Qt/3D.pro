#-------------------------------------------------
#
# Project created by QtCreator 2018-09-26T20:39:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3D
TEMPLATE = app
CONFIG += c++1z
LIBS += -lOpenCL


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
  ../../auxiliar/objparser.cpp \
  ../../auxiliar/multithreadmanager.cpp \
  ../../engine/projector.cpp \
  ../../engine/rasteriser/abstractrasteriser.cpp \
  ../../engine/rasteriser/abstractrasteriserCPU.cpp \
  ../../engine/rasteriser/abstractrasteriserGPU.cpp \
  #../../engine/rasteriser/rasteriseropencl.cpp \
  #../../engine/rasteriser/rasteriserwireframe.cpp \
        src/main.cpp \
        src/mainwindow.cpp \
        canvas.cpp \
  ../../engine/rasteriser/rasteriserinterpolatedvertex.cpp \
  #../../engine/rasteriser/rasteriserflat.cpp \
  src/openclhandler.cpp \
        src/renderengine.cpp \
	../../engine/world.cpp \
	../../engine/camera.cpp \
    ../../engine/math/matrix.cpp \
    ../../engine/spatial/mesh.cpp \

HEADERS += \
  ../../auxiliar/objparser.h \
  ../../auxiliar/multithreadmanager.h \
  ../../engine/projector.h \
  ../../engine/rasteriser/abstractrasteriser.h \
  ../../engine/rasteriser/abstractrasteriserCPU.h \
  ../../engine/rasteriser/abstractrasteriserGPU.h \
  #../../engine/rasteriser/rasteriseropencl.h \
  #../../engine/rasteriser/rasteriserwireframe.h \
        src/mainwindow.h \
        canvas.h \
  ../../engine/rasteriser/rasteriserinterpolatedvertex.h \
  #../../engine/rasteriser/rasteriserflat.h \
  src/openclhandler.h \
        src/renderengine.h \
	../../engine/world.h \
	../../engine/camera.h \
    ../../engine/math/matrix.h \
    ../../engine/math/point3.h \
    ../../engine/math/vector3.h \
    ../../engine/math/matrix3.h \
    ../../engine/spatial/face3.h \
    ../../engine/spatial/spatial.h \
    ../../engine/spatial/mesh.h \
    ../../engine/math/point2.h \
    ../../engine/math/matrix2.h \
    ../../engine/planar/rect.h \
    ../../engine/planar/triangle.h \
    ../../engine/planar/vertex.h \

FORMS += \
        mainwindow.ui
