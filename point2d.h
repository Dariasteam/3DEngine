#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>
#include <matrix.h>

#include "point3d.h"

struct Point2 : public Matrix {

  Point2 () : Point2 (0, 0) {}
  Point2 (const Point2& p) : Point2 (p.x(), p.y()) {}
  Point2 (const std::vector<double>& v) : Point2 (v[0], v[1]) {}
  Point2 (const Matrix& mtx) : Matrix (mtx) {}
  Point2 (double x, double y) : Matrix (1, 2) {
    matrix[0][0] = x;
    matrix[0][1] = y;
  }

  double& operator[] (unsigned i) { return matrix[0][i]; }
  double  operator[] (unsigned i) const { return matrix[0][i]; }

  inline double x() const { return matrix[0][0];}
  inline double y() const { return matrix[0][1];}
};

struct Matrix2 : public Matrix {

  Matrix2 (const Matrix& mtx) : Matrix (mtx) {}

  Matrix2 () : Matrix (
              {{0, 0},
               {0, 0}}) {}

  Matrix2 (const Point2& a,
           const Point2& b) : Matrix (2, 2) {
    matrix[0] = a.matrix[0];
    matrix[1] = b.matrix[0];
  }

  void operator= (const Matrix& mtx) {
    matrix = mtx.matrix;
  }
  inline Point2 a() const { return matrix[0]; }
  inline Point2 b() const { return matrix[1]; }
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
};


#endif // POINT2D_H


