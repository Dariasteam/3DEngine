#ifndef VECTOR2UNITARY_H
#define VECTOR2UNITARY_

#include <climits>
#include "point2.h"

/*
struct Vector2Unitary : public P2<signed char> {
  Vector2Unitary() : P2 (0, 0) {}
  Vector2Unitary (double x, double y) : P2(
    x * SCHAR_MAX,
    y * SCHAR_MAX
  )
  {}

  Vector2Unitary (int x, int y) : P2 (x, y) {}

  Vector2 toVector2 () const {
    return Vector2 {
      double(X) / SCHAR_MAX,
      double(Y) / SCHAR_MAX,
    };
  }


};

//typedef Vector2Unitary Normal2;
*/
#endif // VECTOR2UNITARY_H