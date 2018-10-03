#ifndef POINT3D_H
#define POINT3D_H

#include "matrix.h"

#include <cmath>
#include <vector>

struct AbstractMatrix {
  std::vector<std::vector<double>> matrix;

  inline unsigned size_v () const { return matrix.size(); }
  inline unsigned size_h () const { return matrix[0].size(); }

  inline double get (unsigned i, unsigned j) const { return matrix[i][j]; }
  inline void set (unsigned i, unsigned j, double v) { matrix[i][j] = v;  }

  AbstractMatrix (unsigned rows, unsigned cols) {
    matrix.resize(rows);
    for (auto& row : matrix)
      row.resize(cols);
  }

  AbstractMatrix (const AbstractMatrix& mtx) {
    matrix = mtx.matrix;
  }

  AbstractMatrix operator+ (const AbstractMatrix& mtx) const {
    unsigned n_rows = size_v();
    unsigned n_cols = size_h();

    AbstractMatrix aux (n_rows, n_cols);

    if (mtx.size_h() == size_h() && mtx.size_v() == size_v()) {
      for (unsigned i = 0; i < n_rows; i++) {
        for (unsigned j = 0; j < n_cols; j++) {
          aux.set(i, j, mtx.get(i, j) + get(i, j));
        }
      }
    }
    return aux;
  }

  void operator+= (const AbstractMatrix& mtx) {
    unsigned n_rows = size_v();
    unsigned n_cols = size_h();

    if (mtx.size_h() == size_h() && mtx.size_v() == size_v()) {
      for (unsigned i = 0; i < n_rows; i++) {
        for (unsigned j = 0; j < n_cols; j++) {
          matrix[i][j] += mtx.get(i, j);
        }
      }
    }
  }

  void operator= (const AbstractMatrix& mtx) {
    unsigned n_rows = mtx.size_v();
    unsigned n_cols = mtx.size_h();

    matrix.clear();
    matrix.resize(n_rows);
    for (unsigned i = 0; i < n_rows; i++) {
      matrix[i].resize(n_cols);
      for (unsigned j = 0; j < n_cols; j++) {
        matrix[i][j] = mtx.get (i, j);
      }
    }
  }
};

struct Point3 : public AbstractMatrix {

  Point3 () : Point3 (0, 0, 0) {}
  Point3 (double x, double y, double z) : AbstractMatrix (1, 3) {
    matrix[0][0] = x;
    matrix[0][1] = y;
    matrix[0][2] = z;
  }

  double& operator[] (unsigned i) { return matrix[0][i]; }
  double operator[] (unsigned i) const { return matrix[0][i]; }

  inline double& x() { return matrix[0][0];}
  inline double& y() { return matrix[0][1];}
  inline double& z() { return matrix[0][2];}

  inline void x(double v) { matrix[0][0] = v; }
  inline void y(double v) { matrix[0][1] = v; }
  inline void z(double v) { matrix[0][2] = v; }

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
