#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../math/point2.h"
#include "../math/point3.h"
#include "vertex.h"
#include "../planar/texture.h"
#include "../math/vector3.h"

struct Triangle {
  Point2 a;
  Point2 b;
  Point2 c;  

  Vector3 normal;

  Vector3 normal_a;
  Vector3 normal_b;
  Vector3 normal_c;

  double z_value;   // distance to camera

  Triangle (const Triangle& t) :
    a (t.a),
    b (t.b),
    c (t.c),
    z_value (t.z_value)
  {}

  UV uv = {};

  Triangle () {}

  Triangle (const Point2& aa,
              const Point2& bb,
              const Point2& cc,
              const double z) :
    a (aa),
    b (bb),
    c (cc),
    z_value (z)
  {}
};

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
    c (t.c),
    z_value (t.z_value)
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

#endif // TRIANGLE_H
