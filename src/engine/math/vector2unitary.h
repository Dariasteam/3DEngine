#ifndef VECTOR2UNITARY_H
#define VECTOR2UNITARY_

#include "point2i.h"

struct Vector2Unitary : public Vector2i {
  Vector2Unitary() : Vector2i (0, 0) {}
  Vector2Unitary (double x, double y) : Vector2i(
    x * SCHAR_MAX,
    y * SCHAR_MAX
  )
  {}

  Vector2Unitary (int x, int y) : Vector2i (x, y) {}

  Vector2 toVector2 () const {
    return Vector2 {
      double(X) / SCHAR_MAX,
      double(Y) / SCHAR_MAX,
    };
  }
};

typedef Vector2Unitary Normal2;

#endif // VECTOR2UNITARY_H