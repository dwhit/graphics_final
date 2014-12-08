#pragma once
#include "GL/glew.h"
#include <QGLWidget>
#include <QMenuBar>
#include <GL/glu.h>

#include "labcamera.h"
#include "terrain.h"
#include "cs123_lib/transforms.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>


class Lab4Canvas : public QGLWidget
{
    Q_OBJECT

public:

    Lab4Canvas(QWidget *parent = 0);
    virtual ~Lab4Canvas();

protected:
    /**
     * You need to fill this in.
     *
     * Renders the terrain to the canvas. This is where you should make all of your OpenGL calls.
     */
    virtual void paintGL();

    // Other Methods

    /**
     * Called whenever m_camera is modified or the canvas is resized. Sets the current OpenGL projection
     * and modelview matrices to match the values in m_camera.
     */
    void updateCamera();
    /**
     * Generates a new OpenGL texture and loads the image data from the file at the given path into
     * the texture. Makes the new texture active and returns its unique texture ID.
     */
    int loadTexture(const QString &filename);

    /** Sets the initial values of OpenGL state variables used by this lab */
    void initializeGL();
    /** Updates the current OpenGL state to avoid object distortion when the window is resized. */
    void resizeGL(int w, int h);

    /** Allows this canvas to handle mouse input */
    void mousePressEvent(QMouseEvent *event);
    /** Allows this canvas to handle mouse input */
    void mouseMoveEvent(QMouseEvent *event);
    /** Allows this canvas to handle keyboard input */
    void keyPressEvent(QKeyEvent *event);
    /** Called by the UI when sidebar settings are changed */
    virtual void settingsChanged();


    /**
     * Draws a line at each vertex showing the direction of that vertex's normal. You may find
     * this to be a useful tool if you're having trouble getting the lighting to look right.
     * By default, this function is called in paintGL(), but only renders anything if
     * m_renderNormals is true. You do not need to modify this function.
     */
    void drawNormals();

    //Member variables
    /** Whether or not drawNormals() should do anything when called. */
    bool m_renderNormals;

    /** Contains the current position and orientation of the virtual camera */
    LabCamera *m_camera;
    /** Causes the canvas to be redrawn at a regular interval */
    QTimer *m_timer;
    /** How often the canvas is redrawn, in frames per second */
    float m_fps;
    /** Helper for implementing trackball rotation */
    glm::vec2 m_lastMousePos;


    int m_textureId;
    Terrain m_terrain;
    GLuint m_shader;
    Transforms m_transforms;

protected slots:
     void mouseDragEvent(QMouseEvent *event);
     void redraw();

 signals:
      void _mouseDrag(QMouseEvent *);
};
