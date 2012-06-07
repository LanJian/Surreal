#include "world.hpp"
#include <stdlib.h>
#include <time.h>
#include <iostream>

World::World(double x, double z, double length, double width, SceneNode* root)
  : m_length(length), m_width(width), m_x(x), m_z(z)
{
  if(root)
    m_root = root;
  else
    m_root = new SceneNode("world");
}

void World::genWorld(){


  // if too big, split
  if(m_length > 70 || m_width > 70){
    splitWorld();
    return;
  }

  // if too small, empty
  if(m_length < 10 || m_width < 10)
    return;


  // too small for building
  if(m_length < 30 || m_width < 30){
    int roll = rand() % 100;
    //std::cerr << "roll " << roll << std::endl;
    // empty
    if(roll < 45)
      return;

    // split
    if(roll < 95){
      splitWorld();
      return;
    }

    // sphere
    addObject(0);
    return;
  }

  int roll = rand() % 100;
  //std::cerr << "roll " << roll << std::endl;
  // empty
  if(roll < 45)
    return;

  // split
  if(roll < 95){
    splitWorld();
    return;
  }

  addObject();
}

void World::splitWorld(){
  int roll = rand() % 2;
  // x direction
  if(roll == 0){
    //std::cerr << "split x " << m_length << std::endl;
    if(m_length <= 1)
      return;
    roll = rand()%((int)m_length-1) + 1;
    World a(m_x, m_z, roll, m_width, m_root);
    a.genWorld();
    World b(m_x+roll, m_z, m_length-roll, m_width, m_root);
    b.genWorld();
  }

  // z direction
  else if(roll == 1){
    //std::cerr << "split z " << m_width << std::endl;
    if(m_width <= 1)
      return;
    roll = rand()%((int)m_width-1) + 1;
    World a(m_x, m_z, m_length, roll, m_root);
    a.genWorld();
    World b(m_x, m_z+roll, m_length, m_width-roll, m_root);
    b.genWorld();
  }
}

void World::addObject(int mode){
  int roll = rand() % 4;
  if(mode >= 0)
    roll = mode;

  // sphere
  if(roll == 0){
    double y = rand() % 100;
    double r = m_length > m_width ? m_width : m_length;
    r = r/2;
    double x = m_x + r;
    double z = m_z + r;

    //std::cerr << "obj" << x << " " << y << " " << z << " " << r << std::endl;

    SceneNode* sphere1 = new GeometryNode("obj", new Sphere());
    sphere1->translate(Vector3D(x,y,z));
    sphere1->scale(Vector3D(r,r,r));
    sphere1->rotate('x', 90);
    int red = rand() % 400 - 140;
    int green = rand() % 400 - 140;
    int blue = rand() % 400 - 140;
    sphere1->set_material(new PhongMaterial(Colour(0.7+(red/1000.0),0.7+(green/1000.0),0.7+(blue/1000.0)), Colour(1,1,1), 16));

    m_root->add_child(sphere1);
  }

  // building
  else{
    roll = rand() % 4;
    double h = rand() % 30 + 15;
    double l = m_length-2;
    double w = m_width-2;


    double x = m_x + 1;
    double z = m_z + m_width - 1;
    SceneNode* building = new BuildingNode("obj", (BuildingNode::BuildingType)roll, l, w, h);
    building->translate(Vector3D(x,-5,z));

    m_root->add_child(building);
    
  }

}

void World::draw(Point3D pos, double distance, bool doOffset){
  m_root->walk_gl(pos, distance, doOffset);
}

bool World::intersectPlayer(Point3D pos){
  return m_root->intersectPlayer(pos);
}
