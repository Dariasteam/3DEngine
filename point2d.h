#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>
#include <matrix.h>

struct Point2 {
  double x;
  double y;

  double& operator[] (unsigned i) {
    switch (i) {
      case 0:
        return x;
      default:
        return y;
    }
  }

  double operator[] (unsigned i) const {
    switch (i) {
      case 0:
        return x;
      default:
        return y;
    }
  }
};

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
};

struct Basis2 {
  Vector2 a;
  Vector2 b;

  Vector2& operator[] (unsigned i) {
    switch (i) {
      case 0:
        return a;
      default:
        return b;
    }
  }

  Vector2 operator[] (unsigned i) const {
    switch (i) {
      case 0:
        return a;
      default:
        return b;
    }
  }
};

struct Matrix2 {
  Vector2 row_a;
  Vector2 row_b;

  Vector2& operator[] (unsigned i) {
    switch (i) {
      case 0:
        return row_a;
      default:
        return row_b;
    }
  }

  Vector2 operator[] (unsigned i) const {
    switch (i) {
      case 0:
        return row_a;
      default:
        return row_b;
    }
  }

  Point2 multiplicate_by (Point2 p) const {
    /*
    Point2 aux {0, 0};

    for (unsigned i = 0; i < 2; i++) {
      for (unsigned j = 0; j < 2; j++) {
        aux[i] += p[j] * this->operator[](j)[i];
      }
    }
    return aux;
    */

    return{
        p.x * row_a.x + p.y * row_a.y,
        p.x * row_b.x + p.y * row_b.y
      };
  }

};

struct PlanarOps {

  // CHanges from Basis A to B
  static Matrix2 generate_basis_change_matrix (Basis2 A, Basis2 B) {

    Matrix r(1,1);      // result
    Matrix m(2, 3);     // equation matrix

    for (unsigned i = 0; i < 2; i++) {
      for (unsigned j = 0; j < 2; j++) {
        m.set(i, j, B[i][j]);
      }
    }

    Matrix2 basis_matrix;
    Gauss g;

    std::vector <double> solutions (2);

    for (unsigned i = 0; i < 2; i++) {
      // Copy the first vector of current basis in last column
      for (unsigned j = 0; j < 2; j++)
        m.set(j, 2, A[i][j]);

      // Make gauss
      g (m, r);

      // Obtain the solutions
      unsigned size = m.size_v();
      for (unsigned j = 0; j < size; j++)
        basis_matrix[j][i] = r.get(j, size) * r.get(j,j);
    }

    return basis_matrix;
  }
  static Point2 change_basis (Basis2 A, Basis2 B, Point2 element) {
    Matrix2 basis_changer = generate_basis_change_matrix(A, B);
    return basis_changer.multiplicate_by(element);
  }

  static Point2 change_basis (Matrix2 matrix, Point2 element) {    
    return matrix.multiplicate_by(element);
  }

};


#endif // POINT2D_H


