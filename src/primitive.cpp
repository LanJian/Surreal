#include "primitive.hpp"
#include "GL/gl.h"
#include "GL/glu.h"

#define SPHERE 1


bool Sphere::isSphereSetup = false;

void Sphere::setupSphere(){
  glNewList(SPHERE, GL_COMPILE);

  GLUquadric* qsphere = gluNewQuadric();
  gluQuadricDrawStyle(qsphere, GLU_FILL);
  gluQuadricNormals(qsphere, GLU_SMOOTH);
  gluQuadricOrientation(qsphere, GLU_OUTSIDE);
  gluQuadricTexture(qsphere, GL_TRUE);
  gluSphere(qsphere, 1.0, 50, 50);

  glEndList();
  gluDeleteQuadric(qsphere);

  isSphereSetup = true;
}

Primitive::~Primitive()
{
}

Sphere::Sphere(){
  m_bounding = new Point3D[2];
  m_bounding[0] = Point3D(-1,-1,-1);
  m_bounding[1] = Point3D(1,1,1);
}

Sphere::~Sphere()
{
}

void Sphere::walk_gl() const
{
  if(!isSphereSetup)
    Sphere::setupSphere();

  glCallList(SPHERE);
}


Point3D* Sphere::getBounding(){
  return m_bounding;
}
