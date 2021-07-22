#ifndef VECTOR3UNITARY_H
#define VECTOR3UNITARY_H

#include <climits>

#include "vector3.h"

struct Vector3Unitary : public P3<signed char> {
  Vector3Unitary () : P3 (0, 0, 0) {}

  Vector3Unitary (double x, double y, double z) : P3<signed char> (
    x * SCHAR_MAX,
    y * SCHAR_MAX,
    z * SCHAR_MAX) {}

  Vector3Unitary (const Vector3Unitary& vec) : P3<signed char> (vec) {}

  Vector3Unitary (const Vector3& vec)  : P3<signed char> (static_cast<signed char>(vec.X * SCHAR_MAX),
                                                          static_cast<signed char>(vec.Y * SCHAR_MAX),
                                                          static_cast<signed char>(vec.Z * SCHAR_MAX)) {}

  Vector3Unitary (const Point3i& p) : P3<signed char> (static_cast<signed char>(p.X * SCHAR_MAX),
                                                       static_cast<signed char>(p.Y * SCHAR_MAX),
                                                       static_cast<signed char>(p.Z * SCHAR_MAX)) {}

  Vector3Unitary operator/ (double d) const {
    return {x() / d,
            y() / d,
            z() / d};
  }

  inline double operator* (const Vector3Unitary& u) const {
    return x() * u.x() +
           y() * u.y() +
           z() * u.z();
  }


  inline double operator* (const Vector3& u) const {
    return x() * u.x() +
           y() * u.y() +
           z() * u.z();
  }

  inline void operator= (const Vector3Unitary& u) {
    X = u.X;
    Y = u.Y;
    Z = u.Z;
  }

  inline Vector3 toVector3 () const {
    return Vector3 {
      double(X) / SCHAR_MAX,
      double(Y) / SCHAR_MAX,
      double(Z) / SCHAR_MAX
    };
  }
};

typedef Vector3Unitary Normal3;

#endif // VECTOR3UNITARY_H
