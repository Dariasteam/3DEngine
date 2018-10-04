#ifndef POINT3D_H
#define POINT3D_H

#include "matrix.h"

#include <iostream>
#include <cmath>
#include <vector>

struct Matrix {
  std::vector<std::vector<double>> matrix;

  inline unsigned size_rows () const { return matrix.size(); }
  inline unsigned size_cols () const { return matrix[0].size(); }

  inline double get (unsigned i, unsigned j) const { return matrix[i][j]; }
  inline void set (unsigned i, unsigned j, double v) { matrix[i][j] = v;  }

  inline std::vector<double>  operator[] (unsigned i) const { return matrix[i]; }
  inline std::vector<double>& operator[] (unsigned i) { return matrix[i]; }

  Matrix (unsigned rows, unsigned cols) {
    matrix.resize(rows);
    for (auto& row : matrix)
      row.resize(cols, 0);
  }

  Matrix (const Matrix& mtx) {
    matrix = mtx.matrix;
  }

  Matrix (const std::vector<std::vector<double>>& vec) {
    matrix = vec;
  }

  Matrix operator+ (const Matrix& mtx) const {
    unsigned n_rows = size_rows();
    unsigned n_cols = size_cols();

    Matrix result_mtx (n_rows, n_cols);

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

  void operator+= (const Matrix& mtx) {
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

  void operator= (const Matrix& mtx) {
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

  Matrix get_transpose () const {
    unsigned n_rows = size_rows();
    unsigned n_cols = size_cols();

    Matrix result_mtx (n_cols, n_rows);

    for (unsigned i = 0; i < n_rows; i++) {
      for (unsigned j = 0; j < n_cols; j++)
        result_mtx.set(j, i, get (i, j));
    }
    return result_mtx;
  }


  void transpose () {    
    matrix = get_transpose().matrix;
  }



  Matrix operator*(const Matrix& mtx) const {
    unsigned n_cols_a = size_cols();
    unsigned n_rows_a = size_rows();

    unsigned n_cols_b = mtx.size_cols();
    unsigned n_rows_b = mtx.size_rows();

    Matrix result_mtx (n_rows_a, n_cols_b);

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

class Gauss {
private:

  inline void exchange_rows (Matrix& matrix, unsigned a, unsigned b) {
    auto fullrow_1 = matrix[a];
    auto fullrow_2 = matrix[b];

    replace_row(matrix, a, fullrow_2);
    replace_row(matrix, b, fullrow_1);
  }


  void reorder_rows_non_zero_first (Matrix& matrix, unsigned col) {
    unsigned size = matrix.size_rows();

    if (matrix.get(col, col) != 0)
      return;

    for (unsigned i = col + 1; i < size; i++) {
        if (matrix.get(i, col) != 0)
          exchange_rows(matrix, col, i);
      }
  }

  void replace_row (Matrix& matrix, unsigned r, std::vector<double>& row) {
    unsigned size = row.size();
    for (unsigned i = 0; i < size; i++)
      matrix.set(r, i, row[i]);
  }

  void multiply_row (std::vector<double>& row, double value) {
    for (auto& element : row)
      element *= value;
  }

  void add_rows (std::vector<double>& a, const std::vector<double>& b) {
    unsigned size = a.size();
    for (unsigned i = 0; i < size; i++)
      a[i] += b[i];
  }

  void next_step (Matrix& matrix, unsigned col) {
    bool end = false;
    unsigned row = col + 1;
    unsigned size = matrix.size_rows();

    reorder_rows_non_zero_first (matrix, col);

    while (!end) {
      if (matrix.get(row, col) != 0) {

        double a = -matrix.get(col, col);
        double b = matrix.get(row, col);

        auto fullrow1 = matrix[col];
        auto fullrow2 = matrix[row];

        multiply_row(fullrow1, b);
        multiply_row(fullrow2, a);

        add_rows(fullrow2, fullrow1);

        replace_row(matrix, row, fullrow2);

      } else {
        if (row > size - 2)
          end = true;
        else
          row++;
      }
    }
  }

  void next_step_reverse (Matrix& matrix, unsigned col) {
    bool end = false;
    unsigned row = col - 1;
    unsigned size = matrix.size_rows();

    reorder_rows_non_zero_first (matrix, col);

    while (!end) {
      if (matrix.get(row, col) != 0) {

        double a = -matrix.get(col, col);
        double b = matrix.get(row, col);

        std::vector<double> fullrow1 = matrix[col];
        std::vector<double> fullrow2 = matrix[row];

        multiply_row(fullrow1, b);
        multiply_row(fullrow2, a);

        add_rows(fullrow2, fullrow1);

        replace_row(matrix, row, fullrow2);

      } else {
        if (row < 1)
          end = true;
        else
          row--;
      }
    }
  }


public:
  void operator () (const Matrix& init, Matrix& result) {
    Matrix aux_m (init);

    unsigned size = init.size_rows();
    for (unsigned i = 0; i < size - 1; i++)
      next_step(aux_m, i);

    for (unsigned i = size - 1; i >= 1; i--)
      next_step_reverse(aux_m, i);

    result = aux_m;
  }
};

struct Point3 : public Matrix {

  Point3 () : Point3 (0, 0, 0) {}
  Point3 (const Point3& p) : Point3 (p.x(), p.y(), p.z()) {}
  Point3 (const std::vector<double>& v) : Point3 (v[0], v[1], v[2]) {}
  Point3 (const Matrix& mtx) : Matrix (mtx) {}
  Point3 (double x, double y, double z) : Matrix (1, 3) {
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

struct Matrix3 : public Matrix {
  Matrix3 (const Matrix &mtx) : Matrix (mtx) {}
  Matrix3 () : Matrix (
              {{0, 0, 0},
               {0, 0, 0},
               {0, 0, 0}}) {}

  Matrix3 (const Point3& a,
           const Point3& b,
           const Point3& c) : Matrix (3, 3) {
    matrix[0] = a.matrix[0];
    matrix[1] = b.matrix[0];
    matrix[2] = c.matrix[0];
  }

  void operator= (const Matrix& mtx) {
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

struct MatrixOps {

  // Changes from Basis A to B
  static Matrix generate_basis_change_matrix (const Matrix& A, const Matrix& B) {
    unsigned a_n_rows = A.size_rows();
    unsigned a_n_cols = A.size_cols();
    unsigned b_n_rows = B.size_rows();
    unsigned b_n_cols = B.size_cols();

    Matrix r(1,1);                        // result matrix
    Matrix m(a_n_rows, a_n_rows + 1);     // equation matrix
    Matrix3 basis_matrix;                 // basis changer matrix
    Gauss g;
    std::vector <double> solutions (a_n_rows);

    if (!(a_n_rows == a_n_cols && b_n_rows == b_n_cols && a_n_rows == b_n_rows)) {
      std::cerr << "ERROR basis change is not defined" << std::endl;
      return m;
    }

    // Copy values to new matrix
    for (unsigned i = 0; i < a_n_rows; i++) {
      for (unsigned j = 0; j < a_n_rows; j++) {
        m.set(i, j, B[i][j]);
      }
    }    

    for (unsigned i = 0; i < a_n_rows; i++) {
      // Copy the first vector of current basis in last column
      for (unsigned j = 0; j < a_n_rows; j++)
        m.set(j, a_n_rows, A[i][j]);

      // Make gauss
      g (m, r);

      // Obtain the solutions      
      for (unsigned j = 0; j < a_n_rows; j++)
        basis_matrix[j][i] = r.get(j, a_n_rows) / r.get(j,j); // FIXME TODO why *?
    }
    return basis_matrix;
  }

  static Matrix change_basis (Matrix3 matrix, const Matrix& element) {
    return (matrix * element.get_transpose()).get_transpose();
  }
};


struct Spatial {
  Basis3 basis {
          {1, 0, 0},
          {0, 1, 0},
          {0, 0, 1}};

  Point3 position {0, 0, 0};

  void translate_local (Vector3 v) {
    position += v;
  }

  void translate_global (Vector3 v) {

    Basis3 b {
      {1, 0, 0},
      {0, 1, 0},
      {0, 0, 1},
    };

    Matrix3 m = MatrixOps::generate_basis_change_matrix(basis, b);
    MatrixOps::change_basis(m, position);

    position += v;
  }

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

  void rotate_z (double deg) {
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

  Mesh () {}

  Mesh (const Mesh& m) {
    faces = m.faces;
    basis = m.basis;
    position = m.position;
  };

  Mesh* express_in_different_basis (Basis3 new_basis) {
    Mesh* aux_mesh = new Mesh;
    aux_mesh->basis = new_basis;

    // Calcular matriz de cambio de base
    Matrix3 basis_changer = MatrixOps::
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
