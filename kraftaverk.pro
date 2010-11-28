#-------------------------------------------------
#
# Project created by QtCreator 2010-11-06T18:40:21
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = kraftaverk
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    block.cpp \
    world.cpp \
    blockstone.cpp \
    blocktorch.cpp \
    blockbutton.cpp \
    blockwire.cpp \
    displaylist.cpp \
    glwidget.cpp \
    blockdoor.cpp \
    dockblockicon.cpp

HEADERS  += mainwindow.h \
    block.h \
    world.h \
    position.h \
    blockstone.h \
    blocktorch.h \
    helper.h \
    blockbutton.h \
    mathlib/vector4.h \
    mathlib/vector3.h \
    mathlib/vector2.h \
    mathlib/quaternion.h \
    mathlib/matrix4.h \
    mathlib/matrix3.h \
    mathlib/mathlib.h \
    mathlib/angle.h \
    blockwire.h \
    displaylist.h \
    glwidget.h \
    blockdoor.h \
    dockblockicon.h

RESOURCES +=

DEFINES += MATHLIB_NO_IOSTREAM MATHLIB_NO_EXCEPTION
