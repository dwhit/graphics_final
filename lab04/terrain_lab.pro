#-------------------------------------------------
#
# Project created by QtCreator 2011-07-31T12:33:32
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = terrain_lab
TEMPLATE = app

unix:!macx: LIBS += -lGLU

INCLUDEPATH += src glm cs123_lib
DEPENDPATH += src glm cs123_lib

SOURCES += src/mainwindow.cpp \
    src/main.cpp \
    src/canvas.cpp \
    src/cs123_lib/transforms.cpp \
    src/cs123_lib/resourceloader.cpp \
    src/terrain.cpp

HEADERS  += \
    src/mainwindow.h \
    src/labcamera.h \
    src/canvas.h \
    src/cs123_lib/transforms.h \
    src/cs123_lib/resourceloader.h \
    src/terrain.h

FORMS    += src/mainwindow.ui

# Don't add the -pg flag unless you know what you are doing. It makes QThreadPool freeze on Mac OS X
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Waddress \
    -Warray-bounds \
    -Wc++0x-compat \
    -Wchar-subscripts \
    -Wformat \
    -Wmain \
    -Wmissing-braces \
    -Wparentheses \
    -Wreorder \
    -Wreturn-type \
    -Wsequence-point \
    -Wsign-compare \
    -Wstrict-aliasing \
    -Wstrict-overflow=1 \
    -Wswitch \
    -Wtrigraphs \
    -Wuninitialized \
    -Wunused-label \
    -Wunused-variable \
    -Wvolatile-register-var \
    -Wno-extra

# QMAKE_CXX_FLAGS_WARN_ON += -Wunknown-pragmas -Wunused-function -Wmain
macx:QMAKE_CXXFLAGS_WARN_ON -= -Warray-bounds -Wc++0x-compat

OTHER_FILES += \
    shaders/shader.frag \
    shaders/shader.vert

RESOURCES += \
    shaders/shaders.qrc

win32:CONFIG(release, debug|release): LIBS += -L/course/cs123/lib/glew/glew-1.10.0/lib/release/ -lGLEW
else:win32:CONFIG(debug, debug|release): LIBS += -L/course/cs123/lib/glew/glew-1.10.0/lib/debug/ -lGLEW
else:unix: LIBS += -L/course/cs123/lib/glew/glew-1.10.0/lib/ -lGLEW

INCLUDEPATH += /course/cs123/lib/glew/glew-1.10.0/include
DEPENDPATH += /course/cs123/lib/glew/glew-1.10.0/include
