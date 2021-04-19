#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../math/point2.h"
#include "../math/point3.h"
#include "vertex.h"
#include "../planar/texture.h"
#include "../math/vector3.h"

struct Triangle {
  Point3 a;
  Point3 b;
  Point3 c;

  Vector3 normal;

  Vector2 normal_a;
  Vector2 normal_b;
  Vector2 normal_c;

  double plane_eq_d;

  UV uv = {};

  Triangle () {}

  void calculate_plane_equation_d () {
    plane_eq_d = -normal.X * a.X
                 -normal.Y * a.Y
                 -normal.Z * a.Z;
  }
};

#endif // TRIANGLE_H
