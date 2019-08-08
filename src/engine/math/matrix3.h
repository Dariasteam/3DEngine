#ifndef MATRIX3_H
#define MATRIX3_H

#include "matrix.h"
#include "point3.h"

struct Matrix3 : public Matrix {
  Matrix3 (const Matrix &mtx) : Matrix (mtx) {}
  Matrix3 () : Matrix (
              {{0, 0, 0},
               {0, 0, 0},
               {0, 0, 0}}) {}

  Matrix3 (const Point3& a,
           const Point3& b,
           const Point3& c) : Matrix (3, 3) {
    matrix[0][0] = a.x();
    matrix[0][1] = a.y();
    matrix[0][2] = a.z();

    matrix[1][0] = b.x();
    matrix[1][1] = b.y();
    matrix[1][2] = b.z();

    matrix[2][0] = c.x();
    matrix[2][1] = c.y();
    matrix[2][2] = c.z();
  }
};

typedef Matrix3 Basis3;

struct Point3Ops {
  static void change_basis (const Basis3& basis,
                            const Point3& element,
                            Point3& result) {

    double a = (basis[0][0] * element.x() + basis[0][1] * element.y() + basis[0][2] * element.z());
    double b = (basis[1][0] * element.x() + basis[1][1] * element.y() + basis[1][2] * element.z());
    double c = (basis[2][0] * element.x() + basis[2][1] * element.y() + basis[2][2] * element.z());

    result.set_x(a);
    result.set_y(b);
    result.set_z(c);
  }
};

#endif // MATRIX3_H
