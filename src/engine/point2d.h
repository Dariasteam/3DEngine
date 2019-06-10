#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>

#include "matrix.h"
#include "point3d.h"

struct Point2 {
  double X;
  double Y;

  Point2 () : Point2 (0, 0) {}
  Point2 (const Point2& p) : X(p.x()), Y(p.y()) {}
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
};

struct Matrix2 : public Matrix {

  Matrix2 (const Matrix& mtx) : Matrix (mtx) {}

  Matrix2 () : Matrix (
              {{0, 0},
               {0, 0}}) {}

  Matrix2 (const Point2& a,
           const Point2& b) : Matrix (2, 2) {
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

typedef Point2 Vector2;

struct Rect {
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

struct Triangle2 {  
  Point2 a;
  Point2 b;
  Point2 c;

  double z_value;   // distance to camera
  Color color = {0, 0, 0};

  Triangle2 (const Triangle2& t) :
    a (t.a),
    b (t.b),
    c (t.c),
    z_value (t.z_value),
    color (t.color)
  {}
};


#endif // POINT2D_H


