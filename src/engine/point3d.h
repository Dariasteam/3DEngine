#ifndef POINT3D_H
#define POINT3D_H

#include "matrix.h"

#include <iostream>
#include <cmath>
#include <list>

#define PI 3.14159265

struct Point3 {
  double X;
  double Y;
  double Z;

  Point3 () {}
  Point3 (const Point3& p) :
    X (p.x()),
    Y (p.y()),
    Z (p.z())
  {}
  Point3 (double x, double y, double z) :
    X(x),
    Y(y),
    Z(z)
  {}

  inline void operator= (const Point3& p) {
    set_x(p.x());
    set_y(p.y());
    set_z(p.z());
  }

  inline void operator+= (const Point3& p) {
    set_x(p.x() + x());
    set_y(p.y() + y());
    set_z(p.z() + z());
  }

  inline void operator*= (const double v) {
    set_x(x() * v);
    set_y(y() * v);
    set_z(z() * v);
  }

  inline double x() const { return X;}
  inline double y() const { return Y;}
  inline double z() const { return Z;}

  inline void set_x (double v) { X = v;}
  inline void set_y (double v) { Y = v;}
  inline void set_z (double v) { Z = v;}
};

static double rad2deg (double rad) {
  return rad * 180.0 / PI;
}

static double deg2rad (double deg) {
  return deg * PI / 180.0;
}

typedef Point3 Color;

struct Vector3 : public Point3 {  
  Vector3 () : Point3 (0, 0, 0) {}
  Vector3 (double x, double y, double z) : Point3 (x, y, z) {}

  static inline Vector3 create_vector (const Point3& a, const Point3& b) {
    return {a.x() - b.x(),
            a.y() - b.y(),
            a.z() - b.z()};
  }

  static inline void create_vector (const Point3& a, const Point3& b, Vector3& vec) {
    vec.set_x(a.x() - b.x());
    vec.set_y(a.y() - b.y());
    vec.set_z(a.z() - b.z());
  };

  static Vector3 cross_product (const Vector3& v, const Vector3& u) {
    return Vector3 {
      v.y() * u.z() - u.y() * v.z(),
      v.z() * u.x() - u.z() * v.x(),
      v.x() * u.y() - u.x() * v.y()
    };
  }

  static double vector_module (const Vector3& v) {
    return std::sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
  }

  static double angle_between (const Vector3& v, const Vector3& u) {
    /*
    Vector3 aux_v = v;
    Vector3 aux_u = u;

    aux_u.normalize();
    aux_v.normalize();

    double angle = rad2deg(std::acos(aux_v * aux_u));
    Vector3 axis = Vector3::cross_product(v, u);
    axis.normalize();
    */

    return rad2deg(std::acos((u * v) / (vector_module(u) * vector_module(v))));
  }

  inline void normalize () {
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

  inline double operator* (const Vector3& u) const {
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
    matrix[0][0] = a.x();
    matrix[0][1] = a.y();
    matrix[0][2] = a.z();

    matrix[1][0] = b.x();
    matrix[1][1] = b.y();
    matrix[1][2] = b.z();

    matrix[2][0] = c.x();
    matrix[2][1] = c.y();
    matrix[2][2] = c.z();
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

  void operator+= (const Face3& face) {
    a += face.a;
    b += face.b;
    c += face.c;
  }

};

struct Spatial {
  bool position_changed = false;
  bool base_changed = false;

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
    position_changed = true;
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
    Point3Ops::change_basis(m, position, position);
    position += v;

    position_changed = true;
  }

  void rotate_x (double deg) {
    Matrix3 rotation_matrix {
                              {1, 0, 0},
                              {0, std::cos(deg), -std::sin(deg)},
                              {0, std::sin(deg),  std::cos(deg)}
                            };

    basis = rotation_matrix * basis;
    base_changed = true;
  }

  void rotate_y (double deg) {
    Matrix3 rotation_matrix {
                              {std::cos(deg), 0, std::sin(deg)},
                              {0, 1, 0},
                              {-std::sin(deg), 0, std::cos(deg)}
                            };

    basis = rotation_matrix * basis;
    base_changed = true;
  }

  void rotate_z (double deg) {
    Matrix3 rotation_matrix {
                              {std::cos(deg), -std::sin(deg), 0},
                              {std::sin(deg),  std::cos(deg), 0},
                              {0, 0, 1}
                            };

    basis = rotation_matrix * basis;
    base_changed = true;
  }
};

struct Mesh : public Spatial {  
  std::vector<Face3> local_coordinates_faces;
  std::vector<Face3> global_coordinates_faces;

  std::vector<Mesh*> nested_meshes;
  Color color{0, 0, 0};
  Point3 p{1,1,1};

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
    generate_normals();
    global_coordinates_faces = local_coordinates_faces;
  }

  void copy_faces_local2global () {
    global_coordinates_faces = local_coordinates_faces;
  }

  std::list<Mesh*> express_in_parents_basis (const Basis3& new_basis);
};

#endif // POINT3D_H