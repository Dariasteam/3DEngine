#ifndef VECTOR3UNITARY_H
#define VECTOR3UNITARY_H

#include "point3i.h"
#include "vector3.h"

struct Vector3Unitary : public Point3i {
  Vector3Unitary () : Point3i (0, 0, 0) {}
  Vector3Unitary (int x, int y, int z) : Point3i (x, y, z) {}
  Vector3Unitary (double x, double y, double z) : Point3i (x, y, z) {}
  Vector3Unitary (const Vector3Unitary& vec) : Point3i (vec) {}
  Vector3Unitary (const Vector3& vec)  : Point3i (vec) {}
  Vector3Unitary (const Point3i& p) : Point3i (p) {}

  static inline Vector3Unitary create_vector (const Point3i& a, const Point3i& b) {
    return {a.x() - b.x(),
            a.y() - b.y(),
            a.z() - b.z()};
  }

  static inline void create_vector (const Point3i& a, const Point3i& b, Vector3Unitary& vec) {
    vec.set_x(a.x() - b.x());
    vec.set_y(a.y() - b.y());
    vec.set_z(a.z() - b.z());
  }

  static Vector3Unitary cross_product (const Vector3Unitary& v, const Vector3Unitary& u) {
    return Vector3Unitary {
      v.y() * u.z() - u.y() * v.z(),
      v.z() * u.x() - u.z() * v.x(),
      v.x() * u.y() - u.x() * v.y()
    };
  }

  static double vector_module (const Vector3Unitary& v) {
    return std::sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
  }

  static double angle_between (const Vector3Unitary& v, const Vector3Unitary& u) {
    return rad2deg(std::acos((u * v) / (vector_module(u) * vector_module(v))));
  }

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
