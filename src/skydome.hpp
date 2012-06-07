#ifndef SKYDOME_HPP
#define SKYDOME_HPP

#include "algebra.hpp"
#include "light.hpp"
#include "GL/gl.h"

#define MINUTE 60000
#define CYCLE 5*MINUTE

class SkyDome {
  public:
    SkyDome(double radius, int nLat, int nLong);
    void draw(int time);

    void setDawn(Colour zenith, Colour horizon);
    void setDay(Colour zenith, Colour horizon);

    void setFog(char* filename);

    void loadGradient(char* filename);

    double getT(int time, double start, double duration) {
      double t = time % (CYCLE);
      t = (t-start*CYCLE)/(duration*CYCLE);
      return t;
    }

    Colour getFogColor(int time);
    Point3D* getSunPos(int time);
    Point3D* getMoonPos(int time);
    Light* getLight(int time);

  private:
    double m_radius;
    int m_nLat, m_nLong;
    Vector3D* m_vertices;

    Colour m_dawn[2];
    Colour m_day[2];

    GLuint m_gradient;

    Point3D m_sunPos;

    Colour* m_fogColors;
    int m_nFogColors;

};

#endif
