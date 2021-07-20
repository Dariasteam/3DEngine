#ifndef Point3i_H
#define Point3i_H

#include <cmath>
#include <climits>
#include "point3.h"

// FIXME: This class should not be unitary, only the vector3i one

struct Point3i {
  int X {0};
  int Y {0};
  int Z {0};

  Point3i () {}
  Point3i (const Point3i& p) :
    X (p.x()),
    Y (p.y()),
    Z (p.z())
  {}

  Point3i (double x, double y, double z) :
    X(x * INT_MAX),
    Y(y * INT_MAX),
    Z(z * INT_MAX)
  {}

  Point3i (int x, int y, int z) :
    X(x),
    Y(y),
    Z(z)
  {}

  Point3i (const Point3& p) :
    X (p.x() * INT_MAX),
    Y (p.y() * INT_MAX),
    Z (p.z() * INT_MAX)
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


  inline double x() const { return X;}
  inline double y() const { return Y;}
  inline double z() const { return Z;}

  inline void set_x (double v) { X = v;}
  inline void set_y (double v) { Y = v;}
  inline void set_z (double v) { Z = v;}

  inline Point3 toPoint3 () {
    return Point3 {
      double(X) / INT_MAX,
      double(Y) / INT_MAX,
      double(Z) / INT_MAX
    };
  }
};

#endif // Point3i_H
