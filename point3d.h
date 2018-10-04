#ifndef POINT3D_H
#define POINT3D_H

#include "matrix.h"

#include <iostream>
#include <cmath>

struct Point3 : public Matrix {

  Point3 ()                             : Point3 (0, 0, 0) {}
  Point3 (const Point3& p)              : Point3 (p.x(), p.y(), p.z()) {}
  Point3 (const std::vector<double>& v) : Point3 (v[0], v[1], v[2]) {}
  Point3 (const Matrix& mtx)            : Matrix (mtx) {}
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
  static inline Point3 create_vector (const Point3& a, const Point3& b) {
    return {a.x() - b.x(),
            a.y() - b.y(),
            a.z() - b.z()};
  }
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

  inline Point3 operator[] (unsigned i) const {
    switch (i) {
      case 0:
        return a;
      case 1:
        return b;
      default:
        return c;
    }
  }

  inline Point3& operator[] (unsigned i) {
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

struct Spatial {

  Spatial (const Spatial& sp) :
    basis (sp.basis),
    position (sp.position)
  {}

  Spatial () {}

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
  std::vector<Mesh*> nested_meshes;
  Color color = {0, 0, 0};

  Mesh () {}

  Mesh (unsigned i) : faces(i) {}

  Mesh (const Mesh& m) {
    faces = m.faces;
    basis = m.basis;
    position = m.position;
    color = m.color;
  }

  void add_nested_mesh (Mesh* mesh) {
    nested_meshes.push_back(mesh);
  }

  Mesh* express_in_different_basis (const Basis3& new_basis) const;

  void apply_matrix (const Matrix3& matrix) {
    basis = matrix * basis;
  }
};

#endif // POINT3D_H
