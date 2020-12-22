#ifndef POINT2_H
#define POINT2_H

#include "point3.h"

struct Color888 {
  unsigned char r;
  unsigned char g;
  unsigned char b;

  bool operator != (const Color888& c) const {
    if (r != c.r || g != c.g || b != c.b)
      return true;
    return false;
  }

  explicit Color888 (const Point3& color) :
    r (static_cast<unsigned char>(color.x())),
    g (static_cast<unsigned char>(color.y())),
    b (static_cast<unsigned char>(color.z()))
  {}

  Color888 (unsigned char R,
            unsigned char G,
            unsigned char B) :
    r (R),
    g (G),
    b (B)
  {}
};

struct Point2 {
  double X;
  double Y;

  Color color = {0,0,0};

  Point2 () : Point2 (0, 0) {}
  Point2 (double x, double y) :
    X(x),
    Y(y)
  {}

  inline double x() const { return X;}
  inline double y() const { return Y;}

  inline void set_x (double v) { X = v; }
  inline void set_y (double v) { Y = v; }
  inline void set_values (double vx, double vy) {
    X = vx;
    Y = vy;
  }

  inline Point2 operator- (const Point2& p) const {
    return {x() - p.x(), y() - p.y()};
  }

  inline Point2 operator+ (const Point2& p) const {
    return {x() + p.x(), y() + p.y()};
  }

  inline double operator* (const Point2& p) const {
    return x() * p.x() + y() * p.y();
  }

  inline double operator/ (const Point2& p) const {
    return x() / p.x() + y() / p.y();
  }

  inline Point2 operator* (const double d) const {
    return {x() * d, y() * d};
  }

  inline Point2 operator/ (const double d) const {
    return {x() / d, y() / d};
  }  

  inline void operator+= (const Point2& p)  {
    set_x(x() + p.x());
    set_y(y() + p.y());
  }

  inline void operator-= (const Point2& p)  {
    set_x(x() - p.x());
    set_y(y() - p.y());
  }

  inline void operator*= (const double d) {
    set_x(x() * d);
    set_y(y() * d);
  }

  inline void operator/= (const double d) {
    set_x(x() / d);
    set_y(y() / d);
  }

  static double  vector_module (const Point2& v) {
    return std::sqrt(v.x() * v.x() + v.y() * v.y());
  }
};

typedef Point2 Vector2;

#endif // POINT2_H
