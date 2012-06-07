//---------------------------------------------------------------------------
// Jack Huang
// 20313374
// jx3huang
//---------------------------------------------------------------------------

#include "a2.hpp"

// Return a matrix to represent a counterclockwise rotation of "angle"
// degrees around the axis "axis", where "axis" is one of the
// characters 'x', 'y', or 'z'.
double toRadian(double angle){return angle*PI/180;}
double toDegree(double angle){return angle*180/PI;}

Matrix4x4 rotation(double angle, char axis)
{
  Matrix4x4 r;
  switch(axis){
    case 'x':
      r[1][1] = cos(angle);
      r[1][2] = -sin(angle);
      r[2][1] = sin(angle);
      r[2][2] = cos(angle);
      break;
    case 'y':
      r[0][0] = cos(angle);
      r[0][2] = sin(angle);
      r[2][0] = -sin(angle);
      r[2][2] = cos(angle);
      break;
    case 'z':
      r[0][0] = cos(angle);
      r[0][1] = -sin(angle);
      r[1][0] = sin(angle);
      r[1][1] = cos(angle);
      break;
    default:
      break;
  }
  return r;
}

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 translation(const Vector3D& displacement)
{
  Matrix4x4 t;
  t[0][3] = displacement[0];
  t[1][3] = displacement[1];
  t[2][3] = displacement[2];
  return t;
}

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 scaling(const Vector3D& scale)
{
  Matrix4x4 s;
  s[0][0] = scale[0];
  s[1][1] = scale[1];
  s[2][2] = scale[2];
  return s;
}
