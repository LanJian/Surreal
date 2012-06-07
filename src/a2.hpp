//---------------------------------------------------------------------------
// Jack Huang
// 20313374
// jx3huang
//---------------------------------------------------------------------------

#ifndef CS488_A2_HPP
#define CS488_A2_HPP

#include "algebra.hpp"
#include <math.h>

const double PI = 4*atan(1);

// You should implement these functions, and use them from viewer.cpp

// Return a matrix to represent a counterclockwise rotation of "angle"
// degrees around the axis "axis", where "axis" is one of the
// characters 'x', 'y', or 'z'.
Matrix4x4 rotation(double angle, char axis);

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 translation(const Vector3D& displacement);

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 scaling(const Vector3D& scale);

double toRadian(double angle);
double toDegree(double angle);

#endif
