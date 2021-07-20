#ifndef POINT2_H
#define POINT2_H

#include "../planar/color.h"

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
