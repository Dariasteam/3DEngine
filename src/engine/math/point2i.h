#ifndef POINT2I_H
#define POINT2I_H

#include "point2.h"
#include <climits>

// FIXME Use templates for generating both point2 and point2i

struct Point2i {
  int X;
  int Y;

  Color color = {0,0,0};

  Point2i () : Point2i (0, 0) {}
  Point2i (double x, double y) :
    X (static_cast<int>(x)),
    Y (static_cast<int>(y))
  {}

  Point2i (const Point2& p) :
    X (static_cast<int>(std::round(p.x()))),
    Y (static_cast<int>(std::round(p.y())))
  {}

  Point2i (int x, int y) :
    X(x),
    Y(y)
  {}

  Point2i (unsigned x, unsigned y) :
    X(x),
    Y(y)
  {}

  inline int x() const { return X;}
  inline int y() const { return Y;}

  inline void set_x (double v) { X = v; }
  inline void set_y (double v) { Y = v; }
  inline void set_values (double vx, double vy) {
    X = vx;
    Y = vy;
  }

  inline Point2i operator- (const Point2i& p) const {
    return {x() - p.x(), y() - p.y()};
  }

  inline Point2i operator+ (const Point2i& p) const {
    return {x() + p.x(), y() + p.y()};
  }

  inline double operator* (const Point2i& p) const {
    return x() * p.x() + y() * p.y();
  }

  inline double operator/ (const Point2i& p) const {
    return x() / p.x() + y() / p.y();
  }

  inline Point2i operator* (const double d) const {
    return {x() * d, y() * d};
  }

  inline Point2i operator/ (const double d) const {
    return {x() / d, y() / d};
  }

  inline void operator+= (const Point2i& p)  {
    set_x(x() + p.x());
    set_y(y() + p.y());
  }

  inline void operator-= (const Point2i& p)  {
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

  static double  vector_module (const Point2i& v) {
    return std::sqrt(v.x() * v.x() + v.y() * v.y());
  }
};

typedef Point2i Vector2i;

#endif // POINT2I_H
