#ifndef POINT3_H
#define POINT3_H

#define PI 3.14159265

#include <cmath>

struct Point3 {
  double X {0};
  double Y {0};
  double Z {0};

  Point3 () {}
  Point3 (const Point3& p) :
    X (p.x()),
    Y (p.y()),
    Z (p.z())
  {}
  Point3 (double x, double y, double z) :
    X(x),
    Y(y),
    Z(z)
  {}

  inline void operator= (const Point3& p) {
    set_x(p.x());
    set_y(p.y());
    set_z(p.z());
  }

  inline bool operator== (const Point3& p) const {
    return (x() == p.x() &&
            y() == p.y() &&
            z() == p.z());
  }

  inline void operator+= (const Point3& p) {
    set_x(p.x() + x());
    set_y(p.y() + y());
    set_z(p.z() + z());
  }

  inline void operator-= (const Point3& p) {
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

  inline Point3 operator- (const Point3& v) const {
    return Point3 {
      x() - v.x(),
      y() - v.y(),
      z() - v.z()
    };
  }

  inline Point3 operator/ (const double d) const {
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
};

static double rad2deg (double rad) {
  return rad * 180.0 / PI;
}

static double deg2rad (double deg) {
  return deg * PI / 180.0;
}

typedef Point3 Color;


#endif // POINT3_H
