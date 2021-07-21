#ifndef Point3i_H
#define Point3i_H

#include <cmath>
#include <climits>
#include "point3.h"

// FIXME: This class should not be unitary, only the vector3i one

struct Point3i {
  signed char X {0};
  signed char Y {0};
  signed char Z {0};

  Point3i () {}
  Point3i (const Point3i& p) :
    X (p.x()),
    Y (p.y()),
    Z (p.z())
  {}

  Point3i (double x, double y, double z) :
    X(x * SCHAR_MAX),
    Y(y * SCHAR_MAX),
    Z(z * SCHAR_MAX)
  {}

  Point3i (int x, int y, int z) :
    X(x),
    Y(y),
    Z(z)
  {}

  Point3i (signed char x, signed char y, signed char z) :
    X(x),
    Y(y),
    Z(z)
  {}

  Point3i (const Point3& p) :
    X (p.x() * SCHAR_MAX),
    Y (p.y() * SCHAR_MAX),
    Z (p.z() * SCHAR_MAX)
  {}

  inline void operator= (const Point3i& p) {
    set_x(p.x());
    set_y(p.y());
    set_z(p.z());
  }

  inline bool operator== (const Point3i& p) const {
    return (x() == p.x() &&
            y() == p.y() &&
            z() == p.z());
  }

  inline void operator+= (const Point3i& p) {
    set_x(p.x() + x());
    set_y(p.y() + y());
    set_z(p.z() + z());
  }

  inline void operator-= (const Point3i& p) {
    set_x(x() - p.x());
    set_y(y() - p.y());
    set_z(z() - p.z());
  }

  inline void operator*= (const double v) {
    set_x(x() * v);
    set_y(y() * v);
    set_z(z() * v);
  }

  inline void operator/= (const double v) {
    set_x(x() / v);
    set_y(y() / v);
    set_z(z() / v);
  }

  inline Point3i operator- (const Point3i& v) const {
    return Point3i {
      x() - v.x(),
      y() - v.y(),
      z() - v.z()
    };
  }

  inline Point3i operator/ (const double d) const {
    return {x() / d,
            y() / d,
            z() / d};
  }


  inline signed char x() const { return X;}
  inline signed char y() const { return Y;}
  inline signed char z() const { return Z;}

  inline void set_x (signed char v) { X = v;}
  inline void set_y (signed char v) { Y = v;}
  inline void set_z (signed char v) { Z = v;}

  inline Point3 toPoint3 () {
    return Point3 {
      double(X) / SCHAR_MAX,
      double(Y) / SCHAR_MAX,
      double(Z) / SCHAR_MAX
    };
  }
};

#endif // Point3i_H
