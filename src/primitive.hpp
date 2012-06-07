#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"


class Primitive {
public:
  virtual ~Primitive();
  virtual void walk_gl() const = 0;
  virtual Point3D* getBounding() = 0;
protected:
  Point3D* m_bounding;
};

class Sphere : public Primitive {
public:
  Sphere();
  virtual ~Sphere();
  virtual void walk_gl() const;
  virtual Point3D* getBounding();
private:
  static bool isSphereSetup;
  static void setupSphere();
};

#endif
