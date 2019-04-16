#ifndef POINT3D_H
#define POINT3D_H

#include "matrix.h"

#include <iostream>
#include <cmath>
#include <list>

#define PI 3.14159265

struct Point3 : public Matrix {

  Point3 ()                             : Point3 (0, 0, 0) {}  
  Point3 (const std::vector<double>& v) : Point3 (v[0], v[1], v[2]) {}
  Point3 (const Matrix& mtx)            : Matrix (mtx) {}
  Point3 (const Point3& p)              : Matrix (p) {}
  Point3 (double x, double y, double z) : Matrix (1, 3) {
    matrix[0][0] = x;
    matrix[0][1] = y;
    matrix[0][2] = z;
  }  

  double& operator[] (unsigned i) { return matrix[0][i]; }
  double  operator[] (unsigned i) const { return matrix[0][i]; }

  void operator= (const Point3& p) {
    set_x(p.x());
    set_y(p.y());
    set_z(p.z());
  }

  void operator+= (const Point3& p) {
    set_x(p.x() + x());
    set_y(p.y() + y());
    set_z(p.z() + z());
  }

  inline double x() const { return matrix[0][0];}
  inline double y() const { return matrix[0][1];}
  inline double z() const { return matrix[0][2];}  

  inline void set_x (double v) { matrix[0][0] = v;}
  inline void set_y (double v) { matrix[0][1] = v;}
  inline void set_z (double v) { matrix[0][2] = v;}
};

static double rad2deg (double rad) {
  return rad * 180.0 / PI;
}

static double deg2rad (double deg) {
  return deg * PI / 180.0;
}

struct Vector3 : public Point3 {
  Vector3 (const Matrix& mtx) : Point3 (mtx) {}
  Vector3 () : Point3 (0, 0, 0) {}
  Vector3 (double x, double y, double z) : Point3 (x, y, z) {}

  static inline Vector3 create_vector (const Point3& a, const Point3& b) {
    return {a.x() - b.x(),
            a.y() - b.y(),
            a.z() - b.z()};
  }

  static double vector_module (const Vector3& v) {
    return std::sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
  }

  static double angle_between (const Vector3& v, const Vector3& u) {
    double dot_product = u * v;
    double result = rad2deg(std::acos(dot_product / (vector_module(u) * vector_module(v))));
    if (dot_product < 0)
      result += 180;
    return result;
  }

  void normalize () {
    double module = Vector3::vector_module(*this);
    set_x(x() / module);
    set_y(y() / module);
    set_z(z() / module);
  }

  Vector3 operator/ (double d) const {
    return {x() / d,
            y() / d,
            z() / d};
  }

  double operator* (const Vector3& u) const {
    return x() * u.x() +
           y() * u.y() +
           z() * u.z();
  }  
};

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
};

typedef Matrix3 Basis3;

struct Point3Ops {
  static void change_basis (const Basis3& basis,
                            const Point3& element,
                                  Point3& result);
};

struct Face3 {   
  Point3 a;
  Point3 b;
  Point3 c;
  Vector3 normal;

  inline const Point3& operator[] (unsigned i) const {
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

  void generate_normal ();  

  void operator= (const Face3& face) {
    a = face.a;
    b = face.b;
    c = face.c;
    normal = face.normal;
  }

  void operator+= (const Face3& face) {
    a += face.a;
    b += face.b;
    c += face.c;
  }

};

struct Spatial {
  Spatial (const Spatial& sp) :
    basis (sp.basis),
    position (sp.position)
  {} 

  Spatial () {}

  Spatial (const Basis3& b, const Point3& p) :
    basis (b),
    position (p)
  {}

  Basis3 basis {
          {1, 0, 0},
          {0, 1, 0},
          {0, 0, 1}};

  Point3 position {0, 0, 0};

  void translate_local (const Vector3& v) {
    position += v;
  }

  void translate_global (const Vector3& v) {

    // FIXME: is this usefull for nested meshes?
    Basis3 b {
      {1, 0, 0},
      {0, 1, 0},
      {0, 0, 1},
    };

    Matrix3 m;
    MatrixOps::generate_basis_change_matrix(basis, b, m);
    MatrixOps::change_basis(m, position);
    position += v;
  }

  void rotate_x (double deg) {
    Matrix3 rotation_matrix {
                              {1, 0, 0},
                              {0, std::cos(deg), -std::sin(deg)},
                              {0, std::sin(deg),  std::cos(deg)}
                            };

    basis = rotation_matrix * basis;
  }

  void rotate_y (double deg) {
    Matrix3 rotation_matrix {
                              {std::cos(deg), 0, std::sin(deg)},
                              {0, 1, 0},
                              {-std::sin(deg), 0, std::cos(deg)}
                            };

    basis = rotation_matrix * basis;
  }

  void rotate_z (double deg) {
    Matrix3 rotation_matrix {
                              {std::cos(deg), -std::sin(deg), 0},
                              {std::sin(deg),  std::cos(deg), 0},
                              {0, 0, 1}
                            };

    basis = rotation_matrix * basis;
  }
};

struct Mesh : public Spatial {
  std::vector<Face3> local_coordinates_faces;
  std::vector<Face3> global_coordinates_faces;

  std::vector<Mesh*> nested_meshes;
  Color color = {0, 0, 0};

  Mesh () {}
  ~Mesh () {}

  Mesh (unsigned i) :
    local_coordinates_faces(i),
    global_coordinates_faces(i)
  {}

  Mesh (const Mesh& m) :
    Spatial(m.basis, m.position),
    local_coordinates_faces (m.local_coordinates_faces),
    global_coordinates_faces (m.global_coordinates_faces),
    color (m.color)    
  {}

  void add_nested_mesh (Mesh* mesh) {
    nested_meshes.push_back(mesh);
  }

  const std::vector<Mesh*>& get_nested_meshes () const {
    return nested_meshes;
  }

  void generate_normals () {
    for (auto& face : local_coordinates_faces)
      face.generate_normal();
  }  

  void generate_data () {
    global_coordinates_faces.resize(local_coordinates_faces.size());
    generate_normals();
  }

  inline void copy_faces_local2global () {
    unsigned long size = local_coordinates_faces.size();
    for (unsigned long i = 0; i < size; i++)
      global_coordinates_faces[i] = local_coordinates_faces[i];    
  }

  std::list<Mesh*> express_in_parents_basis (const Basis3& new_basis);
};

#endif // POINT3D_H
