#include "camera.h"

Camera::Camera() {
  fuge = {0, 0, 0};
  vector_plane = {0, 0, 1};
  point_plane = {0, 0, 180};

  bounds = {-100, -100, 100, 100};

  position = {0, 0, 0};

  basis = Basis3 ({
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  });

  projected_basis = {
    {1, 0},
    {0, 1}
  };

}

Camera *Camera::express_in_different_basis(const Basis3& new_basis) const {
  Camera* aux_camera = new Camera;
  aux_camera->basis = new_basis;

  // Calcular matriz de cambio de base
  Matrix3 basis_changer = MatrixOps::
                              generate_basis_change_matrix(basis, new_basis);

  // Calcular los puntos de cada cara expresados en la nueva base
  aux_camera->vector_plane = MatrixOps::change_basis(basis_changer,
                                                      aux_camera->vector_plane);
  aux_camera->point_plane = MatrixOps::change_basis(basis_changer,
                                                     aux_camera->point_plane);

  aux_camera->fuge += MatrixOps::change_basis(basis_changer, position);
  aux_camera->point_plane += MatrixOps::change_basis(basis_changer, position);

  return aux_camera;
}

/*
void Camera::rotate_z(double deg) {
  Matrix2 rotation_matrix_2 {
    {std::cos(deg), -std::sin(deg)},
    {std::sin(deg), std::cos(deg)},
  };

  projected_basis.a = rotation_matrix_2.multiplicate_by(projected_basis.a);
  projected_basis.b = rotation_matrix_2.multiplicate_by(projected_basis.b);
}
*/
