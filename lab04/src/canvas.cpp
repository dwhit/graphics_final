#include "canvas.h"

#ifdef __APPLE__
#include <OpenGL/glext.h>
#else
#ifndef __WIN32
#include <GL/glext.h>
#endif
#endif

#include <qgl.h>
#include <iostream>

#include <cs123_lib/resourceloader.h>

#include <QMouseEvent>
#include <QTimer>
#include <QFile>

#include <glm/glm.hpp>                   // glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/gtc/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale

#include <cmath>

using std::cout;
using std::endl;

// hack to get around windef.h on Windows
#undef near
#undef far

Lab4Canvas::Lab4Canvas(QWidget *parent) : QGLWidget(parent), m_fps(30.0f)
{
    m_renderNormals = false;
    m_camera = new LabCamera();
    m_camera->eye.x = 0.0f, m_camera->eye.y = 20.f, m_camera->eye.z = 10.0f;
    m_camera->center.x = 0.0f, m_camera->center.y = 0.0f, m_camera->center.z = 0.0f;
    m_camera->up.x = 0.0f, m_camera->up.y = 1.0f, m_camera->up.z = 0.0f;
    m_camera->angle = 45.0f, m_camera->near = 1.0f, m_camera->far = 1000.0f;

    // Seed a random number, your terrain will change based on what number you seed.
    srand(0);

    // Do not change these lines.
    QObject::connect(this, SIGNAL(_mouseDrag(QMouseEvent*)),this, SLOT(mouseDragEvent(QMouseEvent*)));
    setAutoBufferSwap(false);
    setFocusPolicy(Qt::StrongFocus);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(redraw()));
    m_timer->start(1000.0f / m_fps);
}

// Destructor
Lab4Canvas::~Lab4Canvas()
{
    if (m_timer)
    {
        delete m_timer;
        m_timer = 0;
    }

    if (m_camera)
    {
        delete m_camera;
        m_camera = 0;
    }
}

/** Sets the initial values of OpenGL state variables used by this lab */
void Lab4Canvas::initializeGL()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      // Problem: glewInit failed, something is seriously wrong.
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    glClearColor(0, 0, 0, 0); // Always reset the screen to black before drawing anything
    glEnable(GL_DEPTH_TEST);  // When drawing a triangle, only keep pixels closer to the camera
                              // than what's already been drawn.

    // Make things pretty.
    glEnable(GL_MULTISAMPLE);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // Cull triangles that are facing away from the camera
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    m_shader = ResourceLoader::loadShaders(":/shaders/shader.vert", ":/shaders/shader.frag");
    m_terrain.initGL(glGetAttribLocation(m_shader, "position"),glGetAttribLocation(m_shader, "normal"), glGetAttribLocation(m_shader, "texCoord"));

    // Load the texture. Feel free to substitue with your own image.
    //m_textureId = loadTexture("/course/cs123/data/image/BoneHead.jpg");
    m_textureId = loadTexture("/home/dwhitney/course/final-template-old/textures/snow1.jpg");
    if (m_textureId == -1)
        cout << "Texture does not exist" << endl;
}

/**
 * You need to fill this in.
 *
 * Renders the terrain to the canvas. This is where you should make all of your OpenGL calls.
 */
void Lab4Canvas::paintGL()
{
    // Clear the color and depth buffers to the current glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Comment these two lines out to use the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glUseProgram(m_shader);
    glUniform3f(glGetUniformLocation(m_shader, "color"), 1, 1, 1);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "mvp"), 1, GL_FALSE, &m_transforms.getTransform()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "m"), 1, GL_FALSE, &m_transforms.model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "v"), 1, GL_FALSE, &m_transforms.view[0][0]);
    glUniform3f(glGetUniformLocation(m_shader, "lightPosition_cameraSpace"),  0.f, 10.f, 1.0f );
    glUniform1i(glGetUniformLocation(m_shader, "textureSampler"), 0);
    m_terrain.draw();
    glUseProgram(0);

    // Force OpenGL to perform all pending operations -- usually a good idea to call this
    glFlush();
    // Swap the buffers to show what we have just drawn onto the screen
    swapBuffers();
}

/** Updates the current OpenGL state to avoid object distortion when the window is resized. */
void Lab4Canvas::resizeGL(int w, int h)
{
    // Prevent a divide by zero
    if (w < 1) w = 1;
    if (h < 1) h = 1;

    // Set the viewport to be the entire window (you probably want to keep this)
    glViewport(0, 0, w, h);
    updateCamera();
}

/**
 * Called whenever m_camera is modified or the canvas is resized. Sets the current OpenGL projection
 * and modelview matrices to match the values in m_camera.
 */
void Lab4Canvas::updateCamera()
{
    float w = width(), h = height();
    float ratio = 1.0f * w / h;

    m_transforms.projection = glm::perspective(m_camera->angle, ratio, m_camera->near, m_camera->far);
    m_transforms.view = glm::lookAt(m_camera->eye, m_camera->center, m_camera->up);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&m_transforms.projection[0][0]);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&m_transforms.view[0][0]);


}

void Lab4Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
        m_lastMousePos.x = event->x(); m_lastMousePos.y = event->y();
}

void Lab4Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
    {
        emit _mouseDrag(event);
        m_lastMousePos.x = event->x(); m_lastMousePos.y = event->y();
    }
}

void Lab4Canvas::mouseDragEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton || event->buttons() & Qt::LeftButton)
    {
        float dx = event->x() - m_lastMousePos.x,
              dy = event->y() - m_lastMousePos.y;

        double x = m_camera->eye.x, y = m_camera->eye.y, z = m_camera->eye.z,
               r = sqrt(m_camera->eye.x * m_camera->eye.x +
                        m_camera->eye.y * m_camera->eye.y +
                        m_camera->eye.z * m_camera->eye.z),
               theta = acos(y / r) - dy * 0.01f,
               phi = atan2(z, x) + dx * 0.01f;

        if (theta > M_PI-.1) theta = M_PI - .1;
        if (theta < .1) theta = .1;

        m_camera->eye.x = r * sin(theta) * cos(phi);
        m_camera->eye.y = r * cos(theta);
        m_camera->eye.z = r * sin(theta) * sin(phi);

        updateCamera();
    }
}

void Lab4Canvas::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Q)
        exit(0);
}

void Lab4Canvas::redraw()
{
    repaint();
}

void Lab4Canvas::settingsChanged() { }

/**
 * Generates a new OpenGL texture and loads the image data from the file at the given path into
 * the texture. Makes the new texture active and returns its unique texture ID.
 */
int Lab4Canvas::loadTexture(const QString &filename)
{
    // Make sure the image file exists
    QFile file(filename);
    if (!file.exists())
        return -1;

    // Load the file into memory
    QImage image;
    image.load(file.fileName());
    image = image.mirrored(false, true);
    QImage texture = QGLWidget::convertToGLFormat(image);

    // Generate a new OpenGL texture ID to put our image into
    GLuint id = 0;
    glGenTextures(1, &id);

    // Make the texture we just created the new active texture
    glBindTexture(GL_TEXTURE_2D, id);

    // Copy the image data into the OpenGL texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    // Set filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set coordinate wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    return id;
}
