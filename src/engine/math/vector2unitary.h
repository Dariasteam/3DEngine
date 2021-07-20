#ifndef VECTOR2UNITARY_H
#define VECTOR2UNITARY_

#include "point2i.h"

struct Vector2Unitary : public Point2i {
  Vector2Unitary() : Point2i (0, 0) {}
  Vector2Unitary (double x, double y) : Point2i(
    x * INT_MAX,
    y * INT_MAX
  )
  {}

  Vector2Unitary (int x, int y) : Point2i (x, y) {}

  Vector2 toVector2 () const {
    return Vector2 {
      double(X) / INT_MAX,
      double(Y) / INT_MAX,
    };
  }
};

typedef Vector2Unitary Normal2;

#endif // VECTOR2UNITARY_H