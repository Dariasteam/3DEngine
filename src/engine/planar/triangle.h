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

  Triangle (const Triangle& t) :
    a (t.a),
    b (t.b),
    c (t.c),

    normal  (t.normal),

    normal_a(t.normal_a),
    normal_b(t.normal_b),
    normal_c(t.normal_c),

    plane_eq_d (t.plane_eq_d)
  {}

  UV uv = {};

  Triangle () {}

  void calculate_plane_equation_d () {
    plane_eq_d = -normal.X * a.X
                 -normal.Y * a.Y
                 -normal.Z * a.Z;
  }
};
/*
struct Triangle2i {
  Point2i a;
  Point2i b;
  Point2i c;

  double z_value;   // distance to camera

  UV uv = {};

  Triangle2i (const Triangle2i& t) :
    a (t.a),
    b (t.b),
    c (t.c),
    z_value (t.z_value)
  {}

  Triangle2i (const Triangle& t) :
    a (t.a),
    b (t.b),
    c (t.c)
  {}

  Triangle2i () {}

  Triangle2i (const Point2i& aa,
              const Point2i& bb,
              const Point2i& cc,
              const double z) :
    a (aa),
    b (bb),
    c (cc),
    z_value (z)
  {}
};
*/
#endif // TRIANGLE_H
