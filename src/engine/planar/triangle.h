#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../math/point2.h"
#include "../math/point3.h"
#include "vertex.h"

struct Triangle2 {
  Point2 a;
  Point2 b;
  Point2 c;

  double z_value;   // distance to camera
  Color888 color = {0, 0, 0};

  Triangle2 (const Triangle2& t) :
    a (t.a),
    b (t.b),
    c (t.c),
    z_value (t.z_value),
    color (t.color)
  {}

  Triangle2 () {}

  Triangle2 (const Point2& aa,
              const Point2& bb,
              const Point2& cc,
              const double z,
              const Color& col) :
    a (aa),
    b (bb),
    c (cc),
    z_value (z),
    color (col)
  {}
};

struct Triangle2i {
  Point2i a;
  Point2i b;
  Point2i c;

  double z_value;   // distance to camera
  Color888 color = {0, 0, 0};

  Triangle2i (const Triangle2i& t) :
    a (t.a),
    b (t.b),
    c (t.c),
    z_value (t.z_value),
    color (t.color)
  {}

  Triangle2i (const Triangle2& t) :
    a (t.a),
    b (t.b),
    c (t.c),
    z_value (t.z_value),
    color (t.color)
  {}

  Triangle2i () {}

  Triangle2i (const Point2i& aa,
              const Point2i& bb,
              const Point2i& cc,
              const double z,
              const Color& col) :
    a (aa),
    b (bb),
    c (cc),
    z_value (z),
    color (col)
  {}
};

#endif // TRIANGLE_H
