#ifndef SPATIAL_H
#define SPATIAL_H

#include "../math/point3.h"
#include "../math/vector3.h"
#include "../math/matrix3.h"

struct Spatial {      
  Spatial (const Spatial& sp) :
    basis (sp.basis),
    position(sp.position)
  {} 

  Spatial () {}

  Spatial (const Basis3& b, const Point3& p) :
    basis (b),
    position (p)
  {}  

  const static Basis3 canonical_base;

  Basis3 basis = canonical_base;

  Point3 position {0, 0, 0};

  void set_position_global (const Vector3& v) {

    // FIXME: is this usefull for nested meshes?
/*
    Matrix3 m;
    MatrixOps::generate_basis_change_matrix(basis, canonical_base, m);
    Point3Ops::change_basis(m, position, position);
*/
    position = v;
  }

  void scale (double s) {
    for (unsigned i = 0; i < 3; i++) {
      for (unsigned j = 0; j < 3; j++) {
        basis[i][j] = basis[i][j] * 3;
      }
    }
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

#endif // SPATIAL_H
