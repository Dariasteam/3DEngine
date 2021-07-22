#ifndef POINT3_H
#define POINT3_H

#define PI 3.14159265

#include <cmath>

template <typename T>
struct P3 {
  T X {0};
  T Y {0};
  T Z {0};

  P3<T> () {}
  P3<T> (const P3<T>& p) :
    X (p.x()),
    Y (p.y()),
    Z (p.z())
  {}
  P3<T> (T x, T y, T z) :
    X(x),
    Y(y),
    Z(z)
  {}

  inline void operator= (const P3<T>& p) {
    set_x(p.x());
    set_y(p.y());
    set_z(p.z());
  }

  inline bool operator== (const P3<T>& p) const {
    return (x() == p.x() &&
            y() == p.y() &&
            z() == p.z());
  }

  inline void operator+= (const P3<T>& p) {
    set_x(p.x() + x());
    set_y(p.y() + y());
    set_z(p.z() + z());
  }

  inline void operator-= (const P3<T>& p) {
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

  inline P3<T> operator- (const P3<T>& v) const {
    return P3 {
      x() - v.x(),
      y() - v.y(),
      z() - v.z()
    };
  }

  inline P3<T> operator/ (const double d) const {
    return {x() / d,
            y() / d,
            z() / d};
  }

  inline T x() const { return X;}
  inline T y() const { return Y;}
  inline T z() const { return Z;}

  inline void set_x (T v) { X = v;}
  inline void set_y (T v) { Y = v;}
  inline void set_z (T v) { Z = v;}
};

static double rad2deg (double rad) {
  return rad * 180.0 / PI;
}

static double deg2rad (double deg) {
  return deg * PI / 180.0;
}

using Point3 = P3<double>;
using Point3i  = P3<int>;
using P3i  = P3<int>;
using Vector3i = P3<int>;

#endif // POINT3_H

