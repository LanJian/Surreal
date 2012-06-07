#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include "algebra.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include <GL/gl.h>

#define BUILDINGTEX 7
#define BUILDINGNOTEX 8

class SceneNode {
public:
  SceneNode(const std::string& name);
  virtual ~SceneNode();

  virtual void walk_gl(Point3D pos, double distance, bool doOffset = false) const;
  virtual bool intersectPlayer(Point3D pos);
  virtual void set_material(Material* material){}

  const Matrix4x4& get_transform() const { return m_trans; }
  const Matrix4x4& get_inverse() const { return m_invtrans; }
  
  void set_transform(const Matrix4x4& m)
  {
    m_trans = m;
    m_invtrans = m.invert();
  }

  void set_transform(const Matrix4x4& m, const Matrix4x4& i)
  {
    m_trans = m;
    m_invtrans = i;
  }

  void add_child(SceneNode* child)
  {
    m_children.push_back(child);
  }

  void remove_child(SceneNode* child)
  {
    m_children.remove(child);
  }

  // Callbacks to be implemented.
  // These will be called from Lua.
  void rotate(char axis, double angle);
  void scale(const Vector3D& amount);
  void translate(const Vector3D& amount);

  // Returns true if and only if this node is a JointNode
  virtual bool is_joint() const;
  
protected:
  
  // Useful for picking
  int m_id;
  std::string m_name;

  // Transformations
  Matrix4x4 m_trans;
  Matrix4x4 m_invtrans;

  // Hierarchy
  typedef std::list<SceneNode*> ChildList;
  ChildList m_children;
};

class JointNode : public SceneNode {
public:
  JointNode(const std::string& name);
  virtual ~JointNode();

  virtual void walk_gl(Point3D pos, double distance, bool doOffset = false) const;

  virtual bool is_joint() const;

  void set_joint_x(double min, double init, double max);
  void set_joint_y(double min, double init, double max);

  struct JointRange {
    double min, init, max;
  };

  
protected:

  JointRange m_joint_x, m_joint_y;
};

class GeometryNode : public SceneNode {
public:
  GeometryNode(const std::string& name,
               Primitive* primitive);
  virtual ~GeometryNode();

  virtual void walk_gl(Point3D pos, double distance, bool doOffset = false) const;
  virtual bool intersectPlayer(Point3D pos);

  const Material* get_material() const;
  Material* get_material();

  virtual void set_material(Material* material)
  {
    m_material = material;
  }

protected:
  Material* m_material;
  Primitive* m_primitive;
};

class BuildingNode : public SceneNode {
  public:
    enum BuildingType{
      GRAY1, GRAY2, RED1, RED2
    };

    BuildingNode(const std::string& name, BuildingType type, double length, double width, double height);
    virtual ~BuildingNode();
    virtual void walk_gl(Point3D pos, double distance, bool doOffset = false) const;
    virtual bool intersectPlayer(Point3D pos);

    static void loadTextures();
    static void setupBuilding();

  private:
    BuildingType m_type;
    double m_length, m_width, m_height;

    static bool isTexLoaded;
    static bool isBuildingSetup;
    //textrues
    static GLuint texGray1;
    static GLuint texGray2;
    static GLuint texRed1;
    static GLuint texRed2;


};

#endif
