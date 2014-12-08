#include "particleemitter.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

ParticleEmitter::ParticleEmitter(glm::vec3 color, glm::vec3 velocity,
                                 glm::vec3 force, float scale, float fuzziness, float speed,
                                 unsigned maxParticles) :
                    m_maxParticles(maxParticles), m_speed(speed),
                    m_fuzziness(fuzziness), m_scale(scale), m_color(color), m_velocity(velocity),
                    m_force(force)
{
    m_particles = new Particle[maxParticles];
    srand (static_cast <unsigned> (time(0)));
    resetParticles();
}

ParticleEmitter::~ParticleEmitter()
{
    if (m_particles)
    {
        delete[] m_particles;
        m_particles = 0;
    }
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteTextures(1, &m_textureID);
}

/**
  * You need to fill this in.
  *
  * Resets the particle at the given index to its initial state. Should reset the
  * position, direction, force, color, life, and decay of the particle.
  */
void ParticleEmitter::resetParticle(unsigned i)
{
    Particle &particle = m_particles[i];

    float rx = urand(-100.f, 100.f);
    float ry = urand(60.f, 70.f);
    float rz = urand(-100.f, 100.f);

    particle.pos = glm::vec3(rx, ry, rz);

    // Continue resetting the particle here...
    particle.life = 5.f;
    particle.decay = urand(0.0025, 0.15);
    particle.color = glm::vec3(1.f, 1.f, 1.f);
    particle.force = glm::vec3(0.f, -3.f, 0.f) + glm::vec3(urand(-m_fuzziness, m_fuzziness),urand(-m_fuzziness, m_fuzziness),urand(-m_fuzziness, m_fuzziness))*glm::vec3(0.01f);
    particle.dir = m_velocity + glm::vec3(urand(-m_fuzziness, m_fuzziness),urand(-m_fuzziness, m_fuzziness),urand(-m_fuzziness, m_fuzziness));
}

/**
  * Resets all particles in this emitter to their initial states
  */
void ParticleEmitter::resetParticles()
{
    for (int i = 0; i < m_maxParticles; i++)
        resetParticle(i);
}

/**
  * You need to fill this in.
  *
  * Performs one step of the particle simulation. Should perform all physics
  * calculations and maintain the life property of each particle.
  */
void ParticleEmitter::updateParticles()
{

    for(int i = 0; i < m_maxParticles; ++i)
    {
        Particle &particle = m_particles[i];

        // Update particle here.
        particle.pos+= particle.dir*glm::vec3(m_speed);
        particle.dir += particle.force;
        particle.life -= particle.decay;
        //if(particle.life < 0) resetParticle(i);
        if(particle.pos.y < -20) resetParticle((i));
    }
}


/**
 * You need to fill this in!
 *
 * Draws each of the particles by making a call to glDrawArrays()
 * for each particle with a different ModelViewProjection matrix.
 */
void ParticleEmitter::drawParticlesVAO(Transforms transform, GLuint mvpLocation, GLuint colorLocation){
    if (!m_isInitialized){
        std::cout << "You must call initGL() before you can draw!" << std::endl;
    } else{
        // Bind the VAO
        glBindVertexArray(m_vao);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);

        GLfloat* color = new GLfloat[4];
        color[0] = 1;
        color[1] = 1;
        color[2] = 1;
        color[3] = 1;


        // @TODO: Render each particle
        for(int i = 0; i < m_maxParticles; ++i)
        {
            Particle &particle = m_particles[i];
            transform.model = glm::translate(glm::mat4(1.f), particle.pos);
            glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(transform.getTransform()));
            glUniform4f(colorLocation, particle.color.x, particle.color.y, particle.color.z, 2*sqrt(particle.life)/*1.f/glm::length(particle.pos)*/);
            glDrawArrays(GL_TRIANGLES, 0, 6);

        }

        glAccum(GL_MULT, 0.5);
        glAccum(GL_ACCUM, 1);
        glAccum(GL_RETURN, 1);


        // Unbind the VAO
        glBindVertexArray(0);
        delete[] color;
        color = NULL;

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);

    }
}

/**
 * You need to fill this in!
 *
 * Sets up a single square that will be drawn (number of Particles) times in different positions
 */
void ParticleEmitter::initGL(int vertexLocation, int textureLocation){
    // Generate and bind VAO and VBO.
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glClear(GL_ACCUM_BUFFER_BIT);

    GLfloat vertexData[] = { -m_scale/2, m_scale/2, 0,
                             0, 0,
                         -m_scale/2, -m_scale/2, 0,
                             0, 1,
                         m_scale/2, m_scale/2, 0,
                             1, 0,
                         -m_scale/2, -m_scale/2, 0,
                             0, 1,
                         m_scale/2, -m_scale/2, 0,
                             1, 1,
                         m_scale/2, m_scale/2, 0,
                           1, 0};



    // @TODO: Fill in the buffer with data for the vertex positions and texture coordinates.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(
       vertexLocation,
       3,                  // num vertices per element (3 for triangle)
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       5*sizeof(GLfloat),                  // stride
       (void*)0            // array buffer offset
    );

    //glBufferData(GL_ARRAY_BUFFER, sizeof(m_particles->), m_particles, GL_STATIC_DRAW);

    glEnableVertexAttribArray(textureLocation);
    glVertexAttribPointer(
       textureLocation,
       2,                  // num vertices per element (3 for triangle)
       GL_FLOAT,           // type
       GL_FALSE,           // normalized?
       5*sizeof(GLfloat),                  // stride
       (void*)(3*sizeof(GLfloat))            // array buffer offset
    );

    //Clean up -- unbind things
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    m_isInitialized = true;
}
