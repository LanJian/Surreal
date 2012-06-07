#include "skydome.hpp"
#include "a2.hpp"
#include "SDL_OGL.h"

SkyDome::SkyDome(double radius, int nLat, int nLong)
  :m_radius(radius), m_nLat(nLat), m_nLong(nLong)
{
  m_vertices = new Vector3D[(nLat+1)*(nLong+1)];
  m_sunPos = Point3D(0,0,0);

  for(int i=0; i<=nLat; i++){
    double theta = i*M_PI/nLat;
    double sintheta = sin(theta);
    double costheta = cos(theta);
    for(int j=0; j<=nLong; j++){
      double phi = j*2*M_PI/nLong;
      double sinphi = sin(phi);
      double cosphi = cos(phi);

      double x = cosphi * sintheta;
      double y = costheta;
      double z = sinphi * sintheta;

      m_vertices[i*(nLat+1)+j] = Vector3D(radius*x, radius*y, radius*z);
    }
  }
}

void SkyDome::loadGradient(char* filename){
  m_gradient = loadTextureBMP(filename);
  setFog(filename);
}

void SkyDome::setDawn(Colour zenith, Colour horizon){
  m_dawn[0] = horizon;
  m_dawn[1] = zenith;
}

void SkyDome::setDay(Colour zenith, Colour horizon){
  m_day[0] = horizon;
  m_day[1] = zenith;
}

void SkyDome::draw(int time){
  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_gradient);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  float U = (float)time/(CYCLE);

  for(int i=0; i<m_nLat; i++){
    for(int j=0; j<m_nLong; j++){
      int a = i*(m_nLong+1) + j;
      int b = a + (m_nLong+1);


      float VTop = (float)(i+1)*2/(m_nLat+3);
      float VBot = (float)(i+2)*2/(m_nLat+3);

      glBegin(GL_TRIANGLES);
      glTexCoord2f(U,VTop);
      glVertex3d(m_vertices[a][0], m_vertices[a][1], m_vertices[a][2]);
      glTexCoord2f(U,VBot);
      glVertex3d(m_vertices[b][0], m_vertices[b][1], m_vertices[b][2]);
      glTexCoord2f(U,VTop);
      glVertex3d(m_vertices[a+1][0], m_vertices[a+1][1], m_vertices[a+1][2]);
      glEnd();

      glBegin(GL_TRIANGLES);
      glTexCoord2f(U,VBot);
      glVertex3d(m_vertices[b][0], m_vertices[b][1], m_vertices[b][2]);
      glVertex3d(m_vertices[b+1][0], m_vertices[b+1][1], m_vertices[b+1][2]);
      glTexCoord2f(U,VTop);
      glVertex3d(m_vertices[a+1][0], m_vertices[a+1][1], m_vertices[a+1][2]);
      glEnd();
    }
  }

  glDisable(GL_TEXTURE_2D);

  // calc sun position
  Point3D* sunPos = getSunPos(time);
  Point3D* moonPos = getMoonPos(time);

  //TODO calc sun color and size
  double sina = sin(getT(time, 0.1, 0.55)*M_PI);
  int sunSize = 2500 - sina*1200;
  Colour sunColor = Colour(1,0.85,0.7)*(1-sina) + Colour(1,1,1)*sina;


  // draw sun
  GLUquadricObj* qsphere = gluNewQuadric();

  gluQuadricDrawStyle(qsphere, GLU_FILL);
  gluQuadricOrientation(qsphere, GLU_OUTSIDE);
  gluQuadricTexture(qsphere, GL_FALSE);

  glColor3f(sunColor.R(),sunColor.G(),sunColor.B());
  glPushMatrix();
  glTranslatef((*sunPos)[0],(*sunPos)[1],(*sunPos)[2]);
  gluSphere(qsphere, sunSize, 20, 20);
  glPopMatrix();

  // draw moon
  GLUquadricObj* qsphere2 = gluNewQuadric();

  gluQuadricDrawStyle(qsphere2, GLU_FILL);
  gluQuadricOrientation(qsphere2, GLU_OUTSIDE);
  gluQuadricTexture(qsphere2, GL_FALSE);

  glColor3f(1,0.9,0.6);
  glPushMatrix();
  glTranslatef((*moonPos)[0],(*moonPos)[1],(*moonPos)[2]);
  gluSphere(qsphere2, 1000, 20, 20);
  glPopMatrix();

}

Colour SkyDome::getFogColor(int time){
  double t = time%(CYCLE);
  t = (t/(CYCLE))*(m_nFogColors-1);
  int i = (int)t;
  t = t-i;
  Colour ret = m_fogColors[i]*(1-t) + m_fogColors[i+1]*t;
  return ret;
}

void SkyDome::setFog(char* filename){
  SDL_Surface* surface = SDL_LoadBMP(filename);
  m_fogColors = new Colour[surface->w];
  m_nFogColors = surface->w;
  for(int i=0; i<surface->w; i++){
    Colour color = getpixel(surface, i, surface->h-1);
    m_fogColors[i] = Colour(color.R()/255, color.G()/255, color.B()/255);
  }
}

Point3D* SkyDome::getSunPos(int time){
  double t = getT(time, 0.1, 0.55);
  double a = t*M_PI;
  if(a > M_PI || a < 0){
    a = 3*M_PI/2;
  }
  double r = m_radius-2000;
  Point3D *ret = new Point3D(r*cos(a), r*sin(a), -150);
  *ret = rotation(toRadian(30), 'x') * (*ret);
  return ret;
}

Point3D* SkyDome::getMoonPos(int time){
  double t = getT(time, 0.53, 0.45);
  double a = t*M_PI;
  if(a > M_PI || a < 0){
    a = 3*M_PI/2;
  }
  double r = m_radius-2000;
  Point3D *ret = new Point3D(r*cos(a), r*sin(a), -150);
  *ret = rotation(toRadian(40), 'x') * (*ret);
  return ret;
}

Light* SkyDome::getLight(int time){

  double t = getT(time, 0.1, 0.55);
  double a = t*M_PI;
  double sina = sin(a);

  Colour black(0,0,0);
  Colour white(1,1,1);

  Light shadow;
  shadow.ambient = black;
  shadow.diffuse = black;
  shadow.specular = black;

  Light lit;
  lit.ambient = black*(1-sina) + sina*Colour(0.2,0.2,0.2);
  lit.diffuse = black*(1-sina) + sina*white;
  lit.specular = black*(1-sina) + sina*white;

  Light* lights = new Light[2];
  lights[0] = shadow;
  lights[1] = lit;
  return lights;

}
