#include "algebra.hpp"
#include <GL/gl.h>

typedef struct
{
  float lifetime;                       // total lifetime of the particle
  float decay;                          // decay speed of the particle
  Colour color;
  Point3D pos;
  Vector3D dir;
  bool active;                       // is particle active or not?
} Particle;


class ParticleSystem{
  public:
    ParticleSystem(int n, Point3D pos, int startTime, GLuint texHandle);
    void update(int time);
    void createParticle(int i);
    void draw();
  private:
    int m_nParticles;
    Particle* m_particles;
    Point3D m_pos;
    int m_lastTime;
    GLuint m_texHandle;
    
};

