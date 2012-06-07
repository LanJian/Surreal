#include "particle.hpp"

ParticleSystem::ParticleSystem(int n, Point3D pos, int startTime, GLuint texHandle)
  : m_nParticles(n), m_pos(pos), m_lastTime(startTime), m_texHandle(texHandle)
{
  m_particles = new Particle[n];
}

void ParticleSystem::update(int time){
  int delta = time-m_lastTime;
  delta = delta/3;

  for(int i=0;i<=m_nParticles;i++){      // evolve the particle parameters
    if(m_particles[i].active && m_particles[i].lifetime>0){
      m_particles[i].lifetime-=m_particles[i].decay*delta;
      m_particles[i].pos = m_particles[i].pos + delta*m_particles[i].dir;
      m_particles[i].dir[1]-=0.00007*delta; // decrease y speed
    } else {
      createParticle(i);
    }
  }

  m_lastTime = time;

}

void ParticleSystem::createParticle(int i){
  m_particles[i].lifetime= (float)(rand() % 500000)/500000.0;
  m_particles[i].decay=0.001;
  m_particles[i].color = Colour(1.0,0.4,0.4);
  m_particles[i].pos = m_pos;
  m_particles[i].dir[0] = 0.01-(float)(rand() % 100)/30000.0;
  m_particles[i].dir[1] = 0.0005-(float)(rand() % 100)/30000.0;
  m_particles[i].dir[2] = 0.0005-(float)(rand() % 100)/30000.0;
  m_particles[i].active = true;

}

void ParticleSystem::draw(){
  float size = 0.006;
  for (int i=0;i<=m_nParticles;i++){
    if(m_particles[i].active && m_particles[i].lifetime>0){
      glColor3f(m_particles[i].color.R(),m_particles[i].color.G(),m_particles[i].color.B());
      glBegin(GL_LINES);

      glVertex3f(m_particles[i].pos[0], m_particles[i].pos[1], m_particles[i].pos[2]);     // top    right
      Point3D p = m_particles[i].pos + 30*m_particles[i].dir;
      glVertex3f(p[0], p[1], p[2]);     // top    right
      glEnd();
    }
  }

}
