#include "matrix.h"

Matrix::~Matrix() {}

Matrix Matrix::operator+(const Matrix &mtx) const {
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

void Matrix::operator+=(const Matrix &mtx) {
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

void Matrix::operator=(const Matrix &mtx) {
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

bool Matrix::operator!=(const Matrix &mtx) const {
  return ! (*this == mtx);
}

bool Matrix::operator==(const Matrix &mtx) const {
  if (size_rows() != mtx.size_rows() ||
      size_cols() != mtx.size_cols()) return false;

  for (unsigned i = 0; i < n_rows; i++)
    for (unsigned j = 0; j < n_cols; j++)
      if (matrix[i][j] != mtx.matrix[i][j])
        return false;

  return true;
}

Matrix Matrix::get_transpose() const {
  unsigned n_rows = size_rows();
  unsigned n_cols = size_cols();

  Matrix result_mtx (n_cols, n_rows);

  for (unsigned i = 0; i < n_rows; i++) {
    for (unsigned j = 0; j < n_cols; j++)
      result_mtx.set(j, i, get (i, j));
  }
  return result_mtx;
}

void Matrix::resize(unsigned rows, unsigned cols) {
  n_rows = rows;
  n_cols = cols;

  matrix.resize(rows);

for (auto& row : matrix)
  row.resize(cols, 0);

}

Matrix Matrix::operator*(const Matrix &mtx) const {
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

void Gauss::exchange_rows(Matrix &matrix, unsigned a, unsigned b) {
  auto fullrow_1 = matrix[a];

  replace_row(matrix, a, matrix[b]);
  replace_row(matrix, b, fullrow_1);
}

void Gauss::reorder_rows_non_zero_first(Matrix &matrix, unsigned col) {
  unsigned size = matrix.size_rows();

  if (matrix.get(col, col) != 0)
    return;

  for (unsigned i = col + 1; i < size; i++) {
    if (matrix.get(i, col) != 0)
      exchange_rows(matrix, col, i);
  }
}

void Gauss::replace_row(Matrix &matrix, unsigned r, std::vector<double> &row) {
  unsigned size = row.size();
  for (unsigned i = 0; i < size; i++)
    matrix.set(r, i, row[i]);
}

void Gauss::multiply_row(std::vector<double> &row, double value) {
  for (auto& element : row)
    element *= value;
}

void Gauss::add_rows(std::vector<double> &a, const std::vector<double> &b) {
  unsigned size = a.size();
  for (unsigned i = 0; i < size; i++)
    a[i] += b[i];
}

void Gauss::operate_gauss(Matrix &matrix, unsigned row, unsigned col) {
  double a = -matrix.get(col, col);
  double b =  matrix.get(row, col);

  std::vector<double> fullrow1 = matrix[col];
  std::vector<double> fullrow2 = matrix[row];

  multiply_row(fullrow1, b);
  multiply_row(fullrow2, a);

  add_rows(fullrow2, fullrow1);

  replace_row(matrix, row, fullrow2);
}

void Gauss::next_step(Matrix &matrix, unsigned col) {
  bool end = false;
  unsigned row = col + 1;
  unsigned size = matrix.size_rows();

  reorder_rows_non_zero_first (matrix, col);

  while (!end) {
    if (matrix.get(row, col) != 0.0) {
      operate_gauss(matrix, row, col);
    } else {
      if (row > size - 2)
        end = true;
      else
        row++;
    }
  }
}

void Gauss::next_step_reverse(Matrix &matrix, unsigned col) {
  bool end = false;
  unsigned row = col - 1;  

  reorder_rows_non_zero_first (matrix, col);

  while (!end) {
    if (matrix.get(row, col) != 0.0) {
        operate_gauss(matrix, row, col);
    } else {
      if (row < 1)
        end = true;
      else
        row--;
    }
  }
}

void Gauss::operator ()(const Matrix &init, Matrix &result) {
  result.resize(init.size_rows(), init.size_rows());
  result = init;

  unsigned size = init.size_rows();
  // Triangulate up
  for (unsigned i = 0; i < size - 1; i++)
    next_step(result, i);

  // Triangulate down
  for (unsigned i = size - 1; i >= 1; i--)
    next_step_reverse(result, i);
}

void MatrixOps::generate_basis_change_matrix(const Matrix &A,
                                             const Matrix &B,
                                             Matrix& result) {
  unsigned a_n_rows = A.size_rows();
  unsigned a_n_cols = A.size_cols();
  unsigned b_n_rows = B.size_rows();
  unsigned b_n_cols = B.size_cols();

  Matrix r;                                 // result matrix
  Matrix m(a_n_rows, a_n_rows + 1);         // equation matrix
  result.resize(a_n_rows, a_n_rows);        // basis changer matrix
  Gauss g;  

  if (!(a_n_rows == a_n_cols && b_n_rows == b_n_cols && a_n_rows == b_n_rows))
    std::cerr << "ERROR basis change is not defined" << std::endl;      

  // Copy values to new matrix
  for (unsigned i = 0; i < a_n_rows; i++) {
    for (unsigned j = 0; j < a_n_rows; j++)
      m.set(i, j, B[i][j]);    
  }

  for (unsigned i = 0; i < a_n_rows; i++) {
    // Copy the first vector of current basis in last column
    for (unsigned j = 0; j < a_n_rows; j++)
      m.set(j, a_n_rows, A.get(j, i));

    // Make gauss
    g (m, r);

    // Obtain the solutions
    for (unsigned j = 0; j < a_n_rows; j++)
      result[j][i] = r.get(j, a_n_cols) / r.get(j, j);
  }  
}

Matrix MatrixOps::change_basis(const Matrix& basis, const Matrix &element) {  
  return element * basis;
}

