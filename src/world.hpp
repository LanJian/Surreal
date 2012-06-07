#include "scene.hpp"
#include "algebra.hpp"

class World{
  public:
    World(double x, double z, double length, double width, SceneNode* root = NULL);
    void genWorld();
    void splitWorld();
    void addObject(int mode = -1);
    void draw(Point3D pos, double distance, bool doOffset = false);
    virtual bool intersectPlayer(Point3D pos);

  private:
    double m_width;
    double m_length;
    double m_x;
    double m_z;
    SceneNode* m_root;
};
