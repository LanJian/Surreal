#include "scene.hpp"
#include "a2.hpp"
#include "SDL_OGL.h"
#include <iostream>
#include <GL/gl.h>

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(Point3D pos, double distance, bool doOffset) const
{
  glPushMatrix();
  glMultMatrixd(get_transform().transpose().begin());

  for(ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ci++){
    (*ci)->walk_gl(pos, distance, doOffset);
  }

  glPopMatrix();
}

bool SceneNode::intersectPlayer(Point3D pos) {
  glPushMatrix();
  glMultMatrixd(get_transform().transpose().begin());
  bool hit = false;
  for(ChildList::const_iterator ci = m_children.begin(); ci != m_children.end(); ci++){
    hit = (*ci)->intersectPlayer(pos);
    if(hit)
      break;
  }
  glPopMatrix();
  return hit;
}

void SceneNode::rotate(char axis, double angle)
{
  set_transform(get_transform()*rotation(toRadian(angle), axis));
}

void SceneNode::scale(const Vector3D& amount)
{
  set_transform(get_transform()*scaling(amount));
}

void SceneNode::translate(const Vector3D& amount)
{
  set_transform(get_transform()*translation(amount));
}

bool SceneNode::is_joint() const
{
  return false;
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(Point3D pos, double distance, bool doOffset) const
{
  SceneNode::walk_gl(pos, distance, doOffset);
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive),
    m_material(NULL)
{
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::walk_gl(Point3D pos, double distance, bool doOffset) const
{
  Point3D p(0,0,0);
  p = get_transform()*p;

  // distance between pos and p
  //double d = (p-pos).length();
  //if(d < distance + 100){

    glPushMatrix();
    glMultMatrixd(get_transform().transpose().begin());
    
    if(m_material)
      m_material->apply_gl();
    // hack
    if(doOffset)
      glScalef(0.993, 0.993, 0.993);
    m_primitive->walk_gl();
    glPopMatrix();
  //}

  SceneNode::walk_gl(pos, distance, doOffset);
}
 
bool GeometryNode::intersectPlayer(Point3D pos) {

  Point3D* bounding = m_primitive->getBounding();

  Point3D primMin = get_transform()*bounding[0];
  Point3D primMax = get_transform()*bounding[1];

  double temp;
  for(int i=0; i<3; i++){
    if(primMin[i] > primMax[i]){
      temp = primMin[i];
      primMin[i] = primMax[i];
      primMax[i] = temp;
    }
  }
  
  Point3D playerMin = Point3D(pos[0]-1, pos[1]-5, pos[2]-1);
  Point3D playerMax = Point3D(pos[0]+1, pos[1]+1, pos[2]+1);


  if( playerMax[0] < primMin[0] || playerMin[0] > primMax[0] ||
      playerMax[1] < primMin[1] || playerMin[1] > primMax[1] ||
      playerMax[2] < primMin[2] || playerMin[2] > primMax[2] ) {
    return false;
  }

  return true;
}

bool BuildingNode::isTexLoaded = false;
bool BuildingNode::isBuildingSetup = false;
GLuint BuildingNode::texGray1;
GLuint BuildingNode::texGray2;
GLuint BuildingNode::texRed1;
GLuint BuildingNode::texRed2;

void BuildingNode::loadTextures(){
  std::cerr << "tex" << std::endl;
  texGray1 = loadTextureBMP("images/gray1.bmp");
  texGray2 = loadTextureBMP("images/gray2.bmp");
  texRed1 = loadTextureBMP("images/red1.bmp");
  texRed2 = loadTextureBMP("images/red2.bmp");
  isTexLoaded = true;
}

void BuildingNode::setupBuilding(){
  glNewList(BUILDINGNOTEX, GL_COMPILE);
  // front face
  glBegin(GL_QUADS);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(1.0, 0.0, 0.0);
  glVertex3d(1.0, 1.0, 0.0);
  glVertex3d(0.0, 1.0, 0.0);

  // back face
  glVertex3d(0.0, 0.0, -1.0);
  glVertex3d(0.0, 1.0, -1.0);
  glVertex3d(1.0, 1.0, -1.0);
  glVertex3d(1.0, 0.0, -1.0);

  // left face
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 1.0, 0.0);
  glVertex3d(0.0, 1.0, -1.0);
  glVertex3d(0.0, 0.0, -1.0);

  // right face
  glVertex3d(1.0, 0.0, 0.0);
  glVertex3d(1.0, 0.0, -1.0);
  glVertex3d(1.0, 1.0, -1.0);
  glVertex3d(1.0, 1.0, 0.0);

  // top face
  glVertex3d(0.0, 1.0, 0.0);
  glVertex3d(1.0, 1.0, 0.0);
  glVertex3d(1.0, 1.0, -1.0);
  glVertex3d(0.0, 1.0, -1.0);

  // bottom face
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, -1.0);
  glVertex3d(1.0, 0.0, -1.0);
  glVertex3d(1.0, 0.0, 0.0);
  glEnd();
  glEndList();
}

BuildingNode::BuildingNode(const std::string& name, BuildingType type, double length, double width, double height)
  : SceneNode(name),
    m_type(type),
    m_length(length),
    m_width(width),
    m_height(height)
{
  if(!isTexLoaded)
    loadTextures();

  if(!isBuildingSetup)
    setupBuilding();
}

BuildingNode::~BuildingNode()
{
}


void BuildingNode::walk_gl(Point3D pos, double distance, bool doOffset) const {
  Point3D p(0,0,0);
  p = get_transform()*p;

  // distance between pos and p
  //double d = (p-pos).length();
  //if(d < distance + 100){
    // texture id
    GLuint texId;
    switch(m_type){
      case GRAY1: texId = texGray1; break;
      case GRAY2: texId = texGray2; break;
      case RED1: texId = texRed1; break;
      case RED2: texId = texRed2; break;
      default: texId = 0; break;
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);


    glColor3d(0.8,0.8,0.8);
    glPushMatrix();

    glMultMatrixd(get_transform().transpose().begin());
    glScaled(m_length, m_height, m_width);

    // hack
    if(doOffset)
      glScalef(0.993, 0.993, 0.993);


    double l = m_length/10;
    double w = m_width/10;
    double h = m_height/10;

    // base
    // front face
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex3d(0.0, 0.0, 0.0);
    glTexCoord2f(l,0);
    glVertex3d(1.0, 0.0, 0.0);
    glTexCoord2f(l,h);
    glVertex3d(1.0, 1.0, 0.0);
    glTexCoord2f(0,h);
    glVertex3d(0.0, 1.0, 0.0);

    // back face
    glTexCoord2f(0,0);
    glVertex3d(0.0, 0.0, -1.0);
    glTexCoord2f(0,h);
    glVertex3d(0.0, 1.0, -1.0);
    glTexCoord2f(l,h);
    glVertex3d(1.0, 1.0, -1.0);
    glTexCoord2f(l,0);
    glVertex3d(1.0, 0.0, -1.0);

    // left face
    glTexCoord2f(w,0);
    glVertex3d(0.0, 0.0, 0.0);
    glTexCoord2f(w,h);
    glVertex3d(0.0, 1.0, 0.0);
    glTexCoord2f(0,h);
    glVertex3d(0.0, 1.0, -1.0);
    glTexCoord2f(0,0);
    glVertex3d(0.0, 0.0, -1.0);

    // right face
    glTexCoord2f(0,0);
    glVertex3d(1.0, 0.0, 0.0);
    glTexCoord2f(w,0);
    glVertex3d(1.0, 0.0, -1.0);
    glTexCoord2f(w,h);
    glVertex3d(1.0, 1.0, -1.0);
    glTexCoord2f(0,h);
    glVertex3d(1.0, 1.0, 0.0);

    glEnd();
    glPopMatrix();

    // roof
    glPushMatrix();

    glMultMatrixd(get_transform().transpose().begin());
    glTranslated(-1.5,m_height,1.5);
    glScaled(m_length+3, 2, m_width+3);
    
    glCallList(BUILDINGNOTEX);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    

  //}

  SceneNode::walk_gl(pos, distance, doOffset);
}

bool BuildingNode::intersectPlayer(Point3D pos){
  Matrix4x4 s = scaling(Vector3D(m_length, m_height, m_width));
  Point3D primMin = get_transform()*s*Point3D(0,0,-1);
  Point3D primMax = get_transform()*s*Point3D(1,1,0);


  double temp;
  for(int i=0; i<3; i++){
    if(primMin[i] > primMax[i]){
      temp = primMin[i];
      primMin[i] = primMax[i];
      primMax[i] = temp;
    }
  }

  //std::cerr << "prim " << primMin << " " <<  primMax << std::endl;
  
  Point3D playerMin = Point3D(pos[0]-1, pos[1]-5, pos[2]-1);
  Point3D playerMax = Point3D(pos[0]+1, pos[1]+1, pos[2]+1);

  //std::cerr << "player " << playerMin << " " << playerMax << std::endl;

  if( playerMax[0] < primMin[0] || playerMin[0] > primMax[0] ||
      playerMax[1] < primMin[1] || playerMin[1] > primMax[1] ||
      playerMax[2] < primMin[2] || playerMin[2] > primMax[2] ) {
    return false;
  }

  return true;
}
