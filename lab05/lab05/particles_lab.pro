#-------------------------------------------------
#
# Project created by QtCreator 2010-06-25T14:35:30
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = particles_lab
TEMPLATE = app

unix:!macx: LIBS += -lGLU

INCLUDEPATH += src glm cs123_lib
DEPENDPATH += src glm cs123_lib

HEADERS  += src/particlewidget.h \
    src/particleemitter.h \
    src/mainwindow.h \
    src/common.h \
    cs123_lib/resourceloader.h \
    cs123_lib/transforms.h

SOURCES += src/particlewidget.cpp \
    src/particleemitter.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    cs123_lib/resourceloader.cpp \
    cs123_lib/transforms.cpp

FORMS    += src/mainwindow.ui

OTHER_FILES += \
    shaders/shader.frag \
    shaders/shader.vert

RESOURCES += \
    textures/images.qrc \
    shaders/shaders.qrc

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -msse4 -ftree-vectorize -ffast-math -fmerge-all-constants -fno-branch-count-reg -foptimize-register-move -funroll-loops

win32:CONFIG(release, debug|release): LIBS += -L/course/cs123/lib/glew/glew-1.10.0/lib/release/ -lGLEW
else:win32:CONFIG(debug, debug|release): LIBS += -L/course/cs123/lib/glew/glew-1.10.0/lib/debug/ -lGLEW
else:unix: LIBS += -L/course/cs123/lib/glew/glew-1.10.0/lib/ -lGLEW

INCLUDEPATH += /course/cs123/lib/glew/glew-1.10.0/include
DEPENDPATH += /course/cs123/lib/glew/glew-1.10.0/include
