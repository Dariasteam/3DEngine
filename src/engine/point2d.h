#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>

#include "matrix.h"
#include "point3d.h"

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

struct Point2F {
  double X;
  double Y;

  Color color = {0,0,0};

  Point2F () : Point2F (0, 0) {}
  Point2F (double x, double y) :
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

  inline Point2F operator- (const Point2F& p) const {
    return {x() - p.x(), y() - p.y()};
  }

  inline Point2F operator+ (const Point2F& p) const {
    return {x() + p.x(), y() + p.y()};
  }

  inline double operator* (const Point2F& p) const {
    return x() * p.x() + y() * p.y();
  }

  inline double operator/ (const Point2F& p) const {
    return x() * p.x() + y() * p.y();
  }

  inline Point2F operator* (const double d) const {
    return {x() * d, y() * d};
  }

  inline Point2F operator/ (const double d) const {
    return {x() / d, y() / d};
  }

  inline void operator+= (const Point2F& p)  {
    set_x(x() + p.x());
    set_y(y() + p.y());
  }

  inline void operator-= (const Point2F& p)  {
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
};

struct Matrix2 : public Matrix {

  Matrix2 (const Matrix& mtx) : Matrix (mtx) {}

  Matrix2 () : Matrix (
              {{0, 0},
               {0, 0}}) {}

  Matrix2 (const Point2F& a,
           const Point2F& b) : Matrix (2, 2) {
    matrix[0][0] = a.x();
    matrix[0][1] = a.y();

    matrix[1][0] = b.x();
    matrix[1][1] = b.y();
  }

  void operator= (const Matrix& mtx) {
    matrix = mtx.matrix;
  }
};

typedef Matrix2 Basis2;

typedef Point2F Vector2;

struct RectF {
  double x;
  double y;
  double height;
  double width;

  double size_x () {
    return std::sqrt(std::pow(x, 2) + std::pow(width, 2));
  }

  double size_y () {
    return std::sqrt(std::pow(x, 2) + std::pow(width, 2));
  }

  double area () {
    return size_x() * size_y();
  }
};

struct Rect {
  unsigned x;
  unsigned y;
  unsigned height;
  unsigned width;

  unsigned size_x () const {
    double v = std::sqrt(std::pow(x, 2) + std::pow(width, 2));
    return static_cast<unsigned>(std::round(v));
  }

  unsigned size_y () const {
    double v = std::sqrt(std::pow(x, 2) + std::pow(width, 2));
    return static_cast<unsigned>(std::round(v));
  }

  unsigned area ()  const {
    return size_x() * size_y();
  }
};

struct Triangle2F {
  Point2F a;
  Point2F b;
  Point2F c;

  double z_value;   // distance to camera
  Color888 color = {0, 0, 0};

  Triangle2F (const Triangle2F& t) :
    a (t.a),
    b (t.b),
    c (t.c),
    z_value (t.z_value),
    color (t.color)
  {}

  Triangle2F () {}

  Triangle2F (const Point2F& aa,
              const Point2F& bb,
              const Point2F& cc,
              const double z,
              const Color& col) :
    a (aa),
    b (bb),
    c (cc),
    z_value (z),
    color (col)
  {}
};

struct Vertex2 {
  int X;
  int Y;

  Color color {0,0,0};

  Vertex2 () : Vertex2 (0, 0, {0,0,0}) {}
  Vertex2 (int x, int y, const Color& c) :
    X(x),
    Y(y),
    color (c)
  {}
  Vertex2 (int x, int y) :
    X(x),
    Y(y)
  {}
  Vertex2 (const Point2F& p) :
    X (static_cast<int>(std::round(p.x()))),
    Y (static_cast<int>(std::round(p.y())))
//    color (c)
  {}
  Vertex2 (const Vertex2& p) :
    X (p.x()),
    Y (p.y()),
    color (p.color)
  {}

  inline int x() const { return X;}
  inline int y() const { return Y;}

  inline void set_x (int v) { X = v; }
  inline void set_y (int v) { Y = v; }
  inline void set_values (int vx, int vy) {
    X = vx;
    Y = vy;
  }

  inline Vertex2 operator- (const Vertex2& p) const {
    return {x() - p.x(), y() - p.y()};
  }

  inline Vertex2 operator+ (const Vertex2& p) const {
    return {x() + p.x(), y() + p.y()};
  }

  inline double operator* (const Vertex2& p) const {
    return x() * p.x() + y() * p.y();
  }

  inline double operator/ (const Vertex2& p) const {
    return x() * p.x() + y() * p.y();
  }

  inline Vertex2 operator* (const int d) const {
    return {x() * d, y() * d};
  }

  inline Vertex2 operator/ (const int d) const {
    return {x() / d, y() / d};
  }

  inline void operator+= (const Vertex2& p)  {
    set_x(x() + p.x());
    set_y(y() + p.y());
  }

  inline void operator-= (const Vertex2& p)  {
    set_x(x() - p.x());
    set_y(y() - p.y());
  }

  inline void operator*= (const int d) {
    set_x(x() * d);
    set_y(y() * d);
  }

  inline void operator/= (const int d) {
    set_x(x() / d);
    set_y(y() / d);
  }
};

struct Triangle2 {
  Vertex2 a;
  Vertex2 b;
  Vertex2 c;

  double z_value;   // distance to camera
  Color888 color = {0, 0, 0};

  Triangle2 (const Triangle2& t) :
    a (t.a),
    b (t.b),
    c (t.c),
    z_value (t.z_value),
    color (t.color)
  {}

  Triangle2 (const Triangle2F& t) :
    a (t.a),
    b (t.b),
    c (t.c),
    z_value (t.z_value),
    color (t.color)
  {}

  Triangle2 () {}

  Triangle2 (const Vertex2& aa,
             const Vertex2& bb,
             const Vertex2& cc,
             const double z,
             const Color& col) :
    a (aa),
    b (bb),
    c (cc),
    z_value (z),
    color (col)
  {}
};

#endif // POINT2D_H


