#include "material.hpp"
#include "GL/gl.h"

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
  : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl() const
{
  // Perform OpenGL calls necessary to set up this material.

  //GLfloat diffuse[] = {m_kd.R(), m_kd.G(), m_kd.B(), 1};
  //GLfloat specular[] = {m_ks.R(), m_ks.G(), m_ks.B(), 1};
  //GLfloat shininess[] = {m_shininess};

  //glEnable(GL_COLOR_MATERIAL);
  glColor3d(m_kd.R(), m_kd.G(), m_kd.B());
  
  //glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  //glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  //glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

}
