#ifndef POINT3D_H
#define POINT3D_H


#include <vector>

struct Point3 {
  double x;
  double y;
  double z;

  Point3 operator+ (const Point3& vec) const {
    return Point3 {
      x + vec.x,
      y + vec.y,
      z + vec.z,
    };
  }

  Point3 operator += (const Point3& vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
  }

};

struct Vector3 {
  double x;
  double y;
  double z;

  Vector3 operator+ (const Vector3& vec) {
    return Vector3 {
      x + vec.x,
      y + vec.y,
      z + vec.z,
    };
  }
};

struct Basis3 {
  Vector3 a;
  Vector3 b;
  Vector3 c;
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

struct Face3 {
  Point3 a;
  Point3 b;
  Point3 c;
};

struct Matrix3 {
  Vector3 row_a = {0, 0, 0};
  Vector3 row_b = {0, 0, 0};
  Vector3 row_c = {0, 0, 0};

  Point3 be_multiplicated_by (Point3 p) {
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

    basis_changer.row_a.x = new_basis.a.x / basis.a.x;
    basis_changer.row_b.y = new_basis.b.y / basis.b.y;
    basis_changer.row_c.z = new_basis.c.z / basis.c.z;



    for (const auto& face : faces) {
      Point3 aux_a = basis_changer.be_multiplicated_by(face.a + position);
      Point3 aux_b = basis_changer.be_multiplicated_by(face.b + position);
      Point3 aux_c = basis_changer.be_multiplicated_by(face.c + position);

      aux_mesh->faces.push_back({aux_a, aux_b, aux_c});
    }

    return aux_mesh;
  }
};

struct SpatialVector : public Spatial{
  double x;
  double y;
  double z;
};

struct SpatialPoint : public Spatial{
  double x;
  double y;
  double z;
};

struct SpatialTriangle : public Spatial {
  Point3 v1;
  Point3 v2;
  Point3 v3;
};

struct SpatialLine : public Spatial {
  SpatialPoint a;
  SpatialPoint b;
};



#endif // POINT3D_H
