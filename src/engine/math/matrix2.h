#ifndef MATRIX2_H
#define MATRIX2_H

#include "matrix.h"
#include "point2.h"

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

#endif // MATRIX2_H
