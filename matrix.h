#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <vector>

class Matrix {
private:
  std::vector<std::vector<double>> matrix;
public:
  Matrix (const std::vector<std::vector<double>>& m) :
    matrix (m)
    {}
  Matrix (unsigned v_size, unsigned h_size) :
    matrix (v_size)
    {
      for (auto& row : matrix) {
        row.resize (h_size, 0);
      }
    }
  Matrix (const Matrix& m) :
    matrix (m.matrix)
  {}

  void rebuild (unsigned size) {
    matrix.clear();
    matrix.resize (size);
    for (auto& row : matrix) {
      row.resize (size, 0);
    }
  }

  void traspose ();

  void operator= (const Matrix& m);

  std::vector<double> operator* (const std::vector<double>& vec);

  std::vector<double> get_row (unsigned i) const { return matrix[i]; }

  inline unsigned size_v () const { return matrix.size(); }
  inline unsigned size_h () const { return matrix[0].size(); }

  inline double get (unsigned i, unsigned j) const { return matrix[i][j]; }
  inline void set (unsigned i, unsigned j, double v) { matrix[i][j] = v;  }

  void print ();
};


class Gauss {
private:

  inline void exchange_rows (Matrix& matrix, unsigned a, unsigned b) {
    auto fullrow_1 = matrix.get_row(a);
    auto fullrow_2 = matrix.get_row(b);

    replace_row(matrix, a, fullrow_2);
    replace_row(matrix, b, fullrow_1);
  }


  void reorder_rows_non_zero_first (Matrix& matrix, unsigned col);

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
    unsigned size = matrix.size_v();

    reorder_rows_non_zero_first (matrix, col);

    while (!end) {
      if (matrix.get(row, col) != 0) {

        double a = -matrix.get(col, col);
        double b = matrix.get(row, col);

        auto fullrow1 = matrix.get_row(col);
        auto fullrow2 = matrix.get_row(row);

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
    unsigned size = matrix.size_v();

    reorder_rows_non_zero_first (matrix, col);

    while (!end) {
      if (matrix.get(row, col) != 0) {

        double a = -matrix.get(col, col);
        double b = matrix.get(row, col);

        auto fullrow1 = matrix.get_row(col);
        auto fullrow2 = matrix.get_row(row);

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

    unsigned size = init.size_v();
    for (unsigned i = 0; i < size - 1; i++)
      next_step(aux_m, i);

    for (unsigned i = size - 1; i >= 1; i--)
      next_step_reverse(aux_m, i);

    result = aux_m;
  }
};

#endif // MATRIX_H
