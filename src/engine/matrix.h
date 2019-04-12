#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>

struct Matrix {
  std::vector<std::vector<double>> matrix;

  inline unsigned size_rows () const { return matrix.size(); }
  inline unsigned size_cols () const { return matrix[0].size(); }

  inline double get (unsigned i, unsigned j) const { return matrix[i][j]; }
  inline void set (unsigned i, unsigned j, double v) { matrix[i][j] = v;  }

  inline const std::vector<double>& operator[] (unsigned i) const { return matrix[i]; }
  inline       std::vector<double>& operator[] (unsigned i) { return matrix[i]; }

  Matrix (unsigned rows, unsigned cols) {
    matrix.resize(rows);
    for (auto& row : matrix)
      row.resize(cols, 0);
  }

  ~Matrix ();

  Matrix (const Matrix& mtx) { matrix = mtx.matrix; }
  Matrix (const std::vector<std::vector<double>>& vec) { matrix = vec; }  

  Matrix operator+ (const Matrix& mtx) const;
  Matrix operator*(const Matrix& mtx) const;
  void operator+= (const Matrix& mtx);
  void operator= (const Matrix& mtx);
  bool operator!= (const Matrix& mtx) const;
  bool operator== (const Matrix& mtx) const;

  Matrix get_transpose () const;
  inline void transpose () { matrix = get_transpose().matrix; }
};

class Gauss {
private:

  inline void exchange_rows (Matrix& matrix, unsigned a, unsigned b);
  inline void reorder_rows_non_zero_first (Matrix& matrix, unsigned col);
  inline void replace_row (Matrix& matrix, unsigned r, std::vector<double>& row);
  inline void multiply_row (std::vector<double>& row, double value);
  inline void add_rows (std::vector<double>& a, const std::vector<double>& b);
  void operate_gauss (Matrix& matrix, unsigned row, unsigned col);
  void next_step (Matrix& matrix, unsigned col);
  void next_step_reverse (Matrix& matrix, unsigned col);

public:
  void operator () (const Matrix& init, Matrix& result);
};

struct MatrixOps {

  // Changes from Basis A to B
  static Matrix generate_basis_change_matrix (const Matrix& A, const Matrix& B);
  static Matrix change_basis (const Matrix& matrix, const Matrix& element);
};

struct Color {
  unsigned r;
  unsigned g;
  unsigned b;
};

#endif // MATRIX_H
