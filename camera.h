#ifndef CAMERA_H
#define CAMERA_H

#include "point3d.h"
#include "point2d.h"

class Camera : public Spatial {
private:

  Plane3 projection_plane;

  Rect bounds;
  void apply_matrix (const Matrix3& matrix);

public:  
  Point3 fuge;
  Vector3 plane;

  Camera();
  inline Point3  get_fuge() { return fuge; }
  inline Vector3 get_plane() { return plane; }
  inline Rect get_bounds() { return bounds; }

  Camera* express_in_different_basis (Basis3 new_basis) {
    Camera* aux_camera = new Camera;
    aux_camera->basis = new_basis;

    // Calcular matriz de cambio de base
    Matrix3 basis_changer;
    for (unsigned i = 0; i < 3; i++)
      basis_changer[i][i] = new_basis[i][i] / basis[i][i];

    // Calcular los puntos de cada cara expresados en la nueva base

    aux_camera->fuge = basis_changer.be_multiplicated_by((fuge + position));
    aux_camera->plane = basis_changer.be_multiplicated_by((plane + position));

    return aux_camera;
  }
};

#endif // CAMERA_H

