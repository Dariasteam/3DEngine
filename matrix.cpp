#include "matrix.h"

void Matrix::traspose() {
  unsigned size = this->size_v();
  for (unsigned i = 0; i < size; i++) {
    for (unsigned j = 0; j <= i; j++) {
      double aux_1 = get(i, j);
      double aux_2 = get(j, i);
      set (i, j, aux_2);
      set (j, i, aux_1);
    }
  }
}

void Matrix::operator=(const Matrix &m) {
  unsigned aux_size_v = m.size_v();
  unsigned aux_size_h = m.size_h();
  matrix.clear();
  matrix.resize (aux_size_v);

  for (unsigned i = 0; i < aux_size_v; i++) {
      matrix[i].resize (aux_size_h);
      for (unsigned j = 0; j < aux_size_h; j++) {
          set (i, j, m.get(i,j));
        }
    }
}

std::vector<double> Matrix::operator*(const std::vector<double> &vec) {
  unsigned size = this->size_v();
  std::vector<double> aux_vec (size, 0);
  for (unsigned i = 0; i < size; i++) {
      for (unsigned j = 0; j < size; j++) {
          aux_vec[i] += vec[j] * this->get(i, j);
        }
    }
  return aux_vec;
}

void Gauss::reorder_rows_non_zero_first(Matrix &matrix, unsigned col) {
  unsigned size = matrix.size_v();

  if (matrix.get(col, col) != 0)
    return;

  for (unsigned i = col + 1; i < size; i++) {
      if (matrix.get(i, col) != 0)
        exchange_rows(matrix, col, i);
    }
}
