#ifndef POINT3D_H
#define POINT3D_H

#include "matrix.h"

#include <iostream>
#include <cmath>
#include <vector>

struct AbstractMatrix {
  std::vector<std::vector<double>> matrix;

  inline unsigned size_rows () const { return matrix.size(); }
  inline unsigned size_cols () const { return matrix[0].size(); }

  inline double get (unsigned i, unsigned j) const { return matrix[i][j]; }
  inline void set (unsigned i, unsigned j, double v) { matrix[i][j] = v;  }

  inline std::vector<double>  operator[] (unsigned i) const { return matrix[i]; }
  inline std::vector<double>& operator[] (unsigned i) { return matrix[i]; }

  AbstractMatrix (unsigned rows, unsigned cols) {
    matrix.resize(rows);
    for (auto& row : matrix)
      row.resize(cols, 0);
  }

  AbstractMatrix (const AbstractMatrix& mtx) {
    matrix = mtx.matrix;
  }

  AbstractMatrix (const std::vector<std::vector<double>>& vec) {
    matrix = vec;
  }

  AbstractMatrix operator+ (const AbstractMatrix& mtx) const {
    unsigned n_rows = size_rows();
    unsigned n_cols = size_cols();

    AbstractMatrix result_mtx (n_rows, n_cols);

    if (mtx.size_cols() == n_cols && mtx.size_rows() == n_rows) {
      for (unsigned i = 0; i < n_rows; i++) {
        for (unsigned j = 0; j < n_cols; j++)
          result_mtx.set(i, j, mtx.get(i, j) + get(i, j));
      }
    } else {
      std::cerr << "ERROR addition is not defined" << std::endl;
    }
    return result_mtx;
  }

  void operator+= (const AbstractMatrix& mtx) {
    unsigned n_rows = size_rows();
    unsigned n_cols = size_cols();

    if (mtx.size_cols() == n_cols && mtx.size_rows() == n_rows) {
      for (unsigned i = 0; i < n_rows; i++) {
        for (unsigned j = 0; j < n_cols; j++)
          matrix[i][j] += mtx.get(i, j);
      }
    } else {
      std::cerr << "ERROR addition is not defined" << std::endl;
    }
  }

  void operator= (const AbstractMatrix& mtx) {
    unsigned n_rows = mtx.size_rows();
    unsigned n_cols = mtx.size_cols();

    matrix.clear();
    matrix.resize(n_rows);
    for (unsigned i = 0; i < n_rows; i++) {
      matrix[i].resize(n_cols);
      for (unsigned j = 0; j < n_cols; j++)
        matrix[i][j] = mtx.get (i, j);
    }
  }

  AbstractMatrix get_transpose () const {
    unsigned n_rows = size_rows();
    unsigned n_cols = size_cols();

    AbstractMatrix result_mtx (n_cols, n_rows);

    for (unsigned i = 0; i < n_rows; i++) {
      for (unsigned j = 0; j < n_cols; j++)
        result_mtx.set(j, i, get (i, j));
    }
    return result_mtx;
  }


  void transpose () {    
    matrix = get_transpose().matrix;
  }



  AbstractMatrix operator*(const AbstractMatrix& mtx) const {
    unsigned n_cols_a = size_cols();
    unsigned n_rows_a = size_rows();

    unsigned n_cols_b = mtx.size_cols();
    unsigned n_rows_b = mtx.size_rows();

    AbstractMatrix result_mtx (n_rows_a, n_cols_b);

    if (n_rows_b == n_cols_a) {
      for (unsigned i = 0; i < n_rows_a; i++) {
        for (unsigned j = 0; j < n_cols_b; j++) {
          for (unsigned k = 0; k < n_cols_a; k++) {
            result_mtx[i][j] += matrix[i][k] * mtx.matrix[k][j];
          }
        }
      }
    } else {
      std::cerr << "ERROR product is not defined" << std::endl;
    }
    return result_mtx;
  }

};

struct Point3 : public AbstractMatrix {

  Point3 () : Point3 (0, 0, 0) {}
  Point3 (const Point3& p) : Point3 (p.x(), p.y(), p.z()) {}
  Point3 (const std::vector<double>& v) : Point3 (v[0], v[1], v[2]) {}
  Point3 (const AbstractMatrix& mtx) : AbstractMatrix (mtx) {}
  Point3 (double x, double y, double z) : AbstractMatrix (1, 3) {
    matrix[0][0] = x;
    matrix[0][1] = y;
    matrix[0][2] = z;
  }

  double& operator[] (unsigned i) { return matrix[0][i]; }
  double  operator[] (unsigned i) const { return matrix[0][i]; }

  inline double x() const { return matrix[0][0];}
  inline double y() const { return matrix[0][1];}
  inline double z() const { return matrix[0][2];}
};

typedef Point3 Vector3;

struct Matrix3 : public AbstractMatrix {

  Matrix3 () : AbstractMatrix (
              {{0, 0, 0},
               {0, 0, 0},
               {0, 0, 0}}) {}

  Matrix3 (const Point3& a,
           const Point3& b,
           const Point3& c) : AbstractMatrix (3, 3) {
    matrix[0] = a.matrix[0];
    matrix[1] = b.matrix[0];
    matrix[2] = c.matrix[0];
  }

  void operator= (const AbstractMatrix& mtx) {
    matrix = mtx.matrix;
  }

  inline Point3 a() const { return matrix[0]; }
  inline Point3 b() const { return matrix[1]; }
  inline Point3 c() const { return matrix[2]; }
};

typedef Matrix3 Basis3;

struct Face3 {
  Point3 a;
  Point3 b;
  Point3 c;

  Point3 operator[] (unsigned i) const {
    switch (i) {
      case 0:
        return a;
      case 1:
        return b;
      default:
        return c;
    }
  }

  Point3& operator[] (unsigned i) {
    switch (i) {
      case 0:
        return a;
      case 1:
        return b;
      default:
        return c;
    }
  }

  void operator= (const Face3& face) {
    a = face.a;
    b = face.b;
    c = face.c;
  }
};

struct SpatialOps {

  // Changes from Basis A to B
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
        basis_matrix[j][i] = r.get(j, size) / r.get(j,j); // FIXME TODO why *?
    }

    return basis_matrix;
  }

  static Point3 change_basis (Matrix3 matrix, const Point3& element) {        
    return (matrix * element.get_transpose()).get_transpose();
  }
};



struct Spatial {
  Basis3 basis;
  Point3 position;  

  void rotate_x (double deg) {
    Matrix3 rotation_matrix ({
                              {1, 0, 0},
                              {0, std::cos(deg), -std::sin(deg)},
                              {0, std::sin(deg),  std::cos(deg)}
                             });

    basis = rotation_matrix * basis;
  }

  void rotate_y (double deg) {
    Matrix3 rotation_matrix ({
                              {std::cos(deg), 0, std::sin(deg)},
                              {0, 1, 0},
                              {-std::sin(deg), 0, std::cos(deg)}
                             });

    basis = rotation_matrix * basis;
  }

  virtual void rotate_z (double deg) {
    Matrix3 rotation_matrix ({
                              {std::cos(deg), -std::sin(deg), 0},
                              {std::sin(deg),  std::cos(deg), 0},
                              {0, 0, 1}
                             });

    basis = rotation_matrix * basis;
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
        aux_face[i] = basis_changer * face[i].get_transpose(); // basis_changer.be_multiplicated_by((face[i]));
        // trasladar a la posición real
        aux_face[i].transpose();
        aux_face[i] += position;
      }

      aux_mesh->faces.push_back(aux_face);
    }

    return aux_mesh;
  }

  void apply_matrix (const Matrix3& matrix) {
    basis = matrix * basis;
  }
};

#endif // POINT3D_H
