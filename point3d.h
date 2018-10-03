#ifndef POINT3D_H
#define POINT3D_H

#include "matrix.h"

#include <cmath>
#include <vector>

struct Point3 {
  double x;
  double y;
  double z;

  Point3 () {}
  Point3 (double X, double Y, double Z) :
     x (X),
     y (Y),
     z (Z)
  {}

  double& operator[] (unsigned i) {
    switch (i) {
      case 0:
        return x;
      case 1:
        return y;
      default:
        return z;
    }
  }

  double operator[] (unsigned i) const {
    switch (i) {
      case 0:
        return x;
      case 1:
        return y;
      default:
        return z;
    }
  }

  Point3 operator+ (const Point3& vec) const {
    return Point3 {
      x + vec.x,
      y + vec.y,
      z + vec.z,
    };
  }

  void operator += (const Point3& vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
  }
};

typedef Point3 Vector3;

struct Basis3 {
  Vector3 a;
  Vector3 b;
  Vector3 c;

  Vector3& operator[] (unsigned i) {
    switch (i) {
      case 0:
        return a;
      case 1:
        return b;
      default:
        return c;
    }
  }

  Vector3 operator[] (unsigned i) const {
    switch (i) {
      case 0:
        return a;
      case 1:
        return b;
      default:
        return c;
    }
  }
};

typedef Basis3 Face3;

struct Matrix3 {
  Vector3 row_a = {0, 0, 0};
  Vector3 row_b = {0, 0, 0};
  Vector3 row_c = {0, 0, 0};

  Vector3& operator[] (unsigned i) {
    switch (i) {
      case 0:
        return row_a;
      case 1:
        return row_b;
      default:
        return row_c;
    }
  }

  Vector3 operator[] (unsigned i) const {
    switch (i) {
      case 0:
        return row_a;
      case 1:
        return row_b;
      default:
        return row_c;
    }
  }

  Point3 multiplicate_by (Point3 p) const {
    Point3 aux {0, 0, 0};

    for (unsigned i = 0; i < 3; i++) {
      for (unsigned j = 0; j < 3; j++)
        aux[i] += this->operator[](i)[j] * p[j];
    }

    return aux;
  }


  Point3 be_multiplicated_by (Point3 p) const {
    Point3 aux {0, 0, 0};

    for (unsigned i = 0; i < 3; i++) {
      for (unsigned j = 0; j < 3; j++) {
        aux[i] += p[j] * this->operator[](j)[i];
      }
    }

    return aux;
  }
};


struct Line3 {
  Point3 point;
  Vector3 vector;
  double parameter;
};

struct Plane3 {
  Vector3 v1;
  Vector3 v2;
  Point3 p;
  double parameter;
};

#define PI 3.14159265359
/*
double deg2rad (double deg) {
  return deg * (PI / 180.0);
}

double rad2deg (double rad) {
  return rad * (180.0 / PI);
}
*/


struct SpatialOps {

  // CHanges from Basis A to B
  static Matrix3 generate_basis_change_matrix (Basis3 A, Basis3 B) {

    Matrix r(1,1);      // result
    Matrix m(3, 4);     // equation matrix

    for (unsigned i = 0; i < 3; i++) {
      for (unsigned j = 0; j < 3; j++) {
        m.set(i, j, B[i][j]);
      }
    }

    Matrix3 basis_matrix;
    Gauss g;

    std::vector <double> solutions (3);

    for (unsigned i = 0; i < 3; i++) {
      // Copy the first vector of current basis in last column
      for (unsigned j = 0; j < 3; j++)
        m.set(j, 3, A[i][j]);

      // Make gauss
      g (m, r);

      // Obtain the solutions
      unsigned size = m.size_v();
      for (unsigned j = 0; j < size; j++)
        basis_matrix[j][i] = r.get(j, size) * r.get(j,j);
    }

    return basis_matrix;
  }
  static Point3 change_basis (Basis3 A, Basis3 B, Point3 element) {
    Matrix3 basis_changer = generate_basis_change_matrix(A, B);
    return basis_changer.be_multiplicated_by(element);
  }
  static Point3 change_basis (Matrix3 matrix, Point3 element) {
    return matrix.be_multiplicated_by(element);
  }
};



struct Spatial {
  Basis3 basis;
  Point3 position;

  virtual void apply_matrix (const Matrix3& matrix) {}

  void rotate_x (double deg) {
    Matrix3 rotation_matrix;
    rotation_matrix.row_a = {1, 0, 0};
    rotation_matrix.row_b = {0, std::cos(deg), -std::sin(deg)};
    rotation_matrix.row_c = {0, std::sin(deg),  std::cos(deg)};

    apply_matrix(rotation_matrix);
  }

  void rotate_y (double deg) {
    Matrix3 rotation_matrix;
    rotation_matrix.row_a = {std::cos(deg), 0, std::sin(deg)};
    rotation_matrix.row_b = {0, 1, 0};
    rotation_matrix.row_c = {-std::sin(deg), 0, std::cos(deg)};

    apply_matrix(rotation_matrix);
  }

  virtual void rotate_z (double deg) {
    Matrix3 rotation_matrix;
    rotation_matrix.row_a = {std::cos(deg), -std::sin(deg), 0};
    rotation_matrix.row_b = {std::sin(deg),  std::cos(deg), 0};
    rotation_matrix.row_c = {0, 0, 1};

    apply_matrix(rotation_matrix);
  }
};

struct Mesh : public Spatial {
  std::vector<Face3> faces;

  Mesh* express_in_different_basis (Basis3 new_basis) {
    Mesh* aux_mesh = new Mesh;
    aux_mesh->basis = new_basis;

    // Calcular matriz de cambio de base
    Matrix3 basis_changer = SpatialOps::
                                generate_basis_change_matrix(basis, new_basis);

    // Calcular los puntos de cada cara expresados en la nueva base
    Face3 aux_face;
    for (const auto& face : faces) {
      for (unsigned i = 0; i < 3; i++) {
        aux_face[i] = basis_changer.be_multiplicated_by((face[i]));
        aux_face[i] += position;
      }

      aux_mesh->faces.push_back(aux_face);
    }

    return aux_mesh;
  }

  void apply_matrix (const Matrix3& matrix) {
    basis[0] = matrix.multiplicate_by(basis[0]);
    basis[1] = matrix.multiplicate_by(basis[1]);
    basis[2] = matrix.multiplicate_by(basis[2]);

/*
    for (auto& face : faces) {
      for (unsigned i = 0; i < 3; i++)
        face[i] = matrix.be_multiplicated_by(face[i]);
    }
*/
  }
};

#endif // POINT3D_H
