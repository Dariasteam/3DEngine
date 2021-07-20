#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../math/point2.h"
#include "../math/point3.h"
#include "../math/point2i.h"
#include "../planar/texture.h"
#include "../math/vector3.h"
#include "../math/vector2unitary.h"
#include "../math/vector3unitary.h"

struct Triangle {
  Point3 a;
  Point3 b;
  Point3 c;

  Normal3 normal;

  Normal2 normal_a;
  Normal2 normal_b;
  Normal2 normal_c;

  double plane_eq_d;

  UV uv = {};

  Triangle () {}
};

#endif // TRIANGLE_H
