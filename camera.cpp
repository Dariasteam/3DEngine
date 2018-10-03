#include "camera.h"

Camera::Camera() {
  fuge = {0, 0, 0};
  vector_plane = {0, 0, 1};
  point_plane = {0, 0, 50};

  bounds = {-50, -50, 50, 50};

  position = {0, 0, 0};

  basis = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  };


}

Camera *Camera::express_in_different_basis(Basis3 new_basis) {
  Camera* aux_camera = new Camera;
  aux_camera->basis = new_basis;


  // Calcular matriz de cambio de base
  Matrix3 basis_changer = SpatialOps::
                              generate_basis_change_matrix(basis, new_basis);

  // Calcular los puntos de cada cara expresados en la nueva base
  aux_camera->vector_plane = basis_changer.be_multiplicated_by((vector_plane));

  aux_camera->fuge = basis_changer.be_multiplicated_by((fuge));
  aux_camera->fuge += position;  

  aux_camera->point_plane = basis_changer.be_multiplicated_by((point_plane));
  aux_camera->point_plane += position;

  return aux_camera;
}

void Camera::apply_matrix(const Matrix3 &matrix) {
  basis.a = matrix.be_multiplicated_by(basis.a);
  basis.b = matrix.be_multiplicated_by(basis.b);
  basis.c = matrix.be_multiplicated_by(basis.c);
}
