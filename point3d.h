#ifndef POINT3D_H
#define POINT3D_H

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

/*
struct Vector3 : public Point3 {
  Vector3 (double x, double y, double z) : Point3 (x, y, z) {}
  Vector3 () {}
};
*/

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

struct Spatial {
  Basis3 basis;
  Point3 position;
};

struct Line3 {
  Point3 point;
  Vector3 vector;
  double parameter;
};

/*
struct Face3 {
  Point3 a;
  Point3 b;
  Point3 c;
};
*/

typedef Basis3 Face3;

#define PI 3.14159265359
/*
double deg2rad (double deg) {
  return deg * (PI / 180.0);
}

double rad2deg (double rad) {
  return rad * (180.0 / PI);
}
*/

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

  Point3 be_multiplicated_by (Point3 p) const {
    Point3 aux;

    aux.x = (row_a.x * p.x + row_a.y * p.y + row_a.z * p.z);
    aux.y = (row_b.x * p.x + row_b.y * p.y + row_b.z * p.z);
    aux.z = (row_c.x * p.x + row_c.y * p.y + row_c.z * p.z);

    return aux;
  }

};

struct Mesh : public Spatial {
  std::vector<Face3> faces;

  Mesh* express_in_different_basis (Basis3 new_basis) {
    Mesh* aux_mesh = new Mesh;
    aux_mesh->basis = new_basis;

    // Calcular matriz de cambio de base
    Matrix3 basis_changer;
    for (unsigned i = 0; i < 3; i++)
      basis_changer[i][i] = new_basis[i][i] / basis[i][i];


    // Calcular los puntos de cada cara expresados en la nueva base
    Face3 aux_face;
    for (const auto& face : faces) {
      for (unsigned i = 0; i < 3; i++)
        aux_face[i] = basis_changer.be_multiplicated_by((face[i] + position));

      aux_mesh->faces.push_back(aux_face);
    }

    return aux_mesh;
  }

  Point3 apply_matrix (const Matrix3& matrix) {
    for (auto& face : faces) {
      for (unsigned i = 0; i < 3; i++)
        face[i] = matrix.be_multiplicated_by(face[i]);
    }
  }

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

  void rotate_z (double deg) {
    Matrix3 rotation_matrix;
    rotation_matrix.row_a = {std::cos(deg), -std::sin(deg), 0};
    rotation_matrix.row_b = {std::sin(deg),  std::cos(deg), 0};
    rotation_matrix.row_c = {0, 0, 1};

    apply_matrix(rotation_matrix);
  }
};

#endif // POINT3D_H
