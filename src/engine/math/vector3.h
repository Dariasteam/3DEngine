#ifndef VECTOR3_H
#define VECTOR3_H

#include "point3.h"

struct Vector3 : public Point3 {
  Vector3 () : Point3 (0, 0, 0) {}
  Vector3 (double x, double y, double z) : Point3 (x, y, z) {}
  Vector3 (const Vector3& vec) : Point3 (vec) {}
  Vector3 (const Point3& p) : Point3 (p) {}

  static inline Vector3 create_vector (const Point3& a, const Point3& b) {
    return {a.x() - b.x(),
            a.y() - b.y(),
            a.z() - b.z()};
  }

  static inline void create_vector (const Point3& a, const Point3& b, Vector3& vec) {
    vec.set_x(a.x() - b.x());
    vec.set_y(a.y() - b.y());
    vec.set_z(a.z() - b.z());
  }

  static Vector3 cross_product (const Vector3& v, const Vector3& u) {
    return Vector3 {
      v.y() * u.z() - u.y() * v.z(),
      v.z() * u.x() - u.z() * v.x(),
      v.x() * u.y() - u.x() * v.y()
    };
  }

  static double vector_module (const Vector3& v) {
    return std::sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
  }

  static double angle_between (const Vector3& v, const Vector3& u) {
    return rad2deg(std::acos((u * v) / (vector_module(u) * vector_module(v))));
  }

  inline void normalize () {
    double module = Vector3::vector_module(*this);
    set_x(x() / module);
    set_y(y() / module);
    set_z(z() / module);
  }

  inline double operator* (const Vector3& u) const {
    return x() * u.x() +
           y() * u.y() +
           z() * u.z();
  }
};

#endif // VECTOR3_H
