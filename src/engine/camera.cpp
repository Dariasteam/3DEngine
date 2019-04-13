#include "camera.h"

Camera::Camera() {
  local_parameters.fuge = {0, 0, 0};
  local_parameters.vector_plane = {0, 0, 1};
  local_parameters.point_plane = {0, 0, 200};

  bounds = {-150, -150, 150, 150};

  position = {0, 0, 0};

  basis = Basis3 ({
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  });
}

void Camera::express_in_different_basis(const Basis3& new_basis) {
  // Calcular matriz de cambio de base
  Matrix3 basis_changer = MatrixOps::generate_basis_change_matrix(basis,
                                                                  new_basis);

  // Calcular los puntos de cada cara expresados en la nueva base
  Point3Ops::change_basis(basis_changer, local_parameters.vector_plane,
                                         global_parameters.vector_plane);
  Point3Ops::change_basis(basis_changer, local_parameters.point_plane,
                                         global_parameters.point_plane);
  Point3Ops::change_basis(basis_changer, local_parameters.fuge,
                                         global_parameters.fuge);

  Point3 translation;
  Point3Ops::change_basis(basis_changer, position, translation);

  global_parameters.fuge        += translation;
  global_parameters.point_plane += translation;
}
