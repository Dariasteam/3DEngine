#include "camera.h"

Camera::Camera() {  
  local_parameters.fuge = {0, 0, 0};
  local_parameters.vector_plane = {0, 0, 1};
  local_parameters.point_plane = {0, 0, 200}; // controls fovy

  global_parameters.fuge         = local_parameters.fuge;
  global_parameters.vector_plane = local_parameters.vector_plane;
  global_parameters.point_plane  = local_parameters.point_plane;

  bounds = {-150, -150, 150, 150};

  position = {0, 0, 0};

  basis = Basis3 ({
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  });
}

#include <iostream>

void Camera::apply_transformation() {
  // Calcular matriz de cambio de base

  /*
  Matrix3 basis_changer;

  Basis3 canonical {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  };
  */

  //MatrixOps::generate_basis_change_matrix(canonical, basis, basis_changer);

  // Calcular los puntos de cada cara expresados en la nueva base
  /*
  Point3Ops::change_basis(basis_changer, local_parameters.vector_plane,
                                         global_parameters.vector_plane);
  Point3Ops::change_basis(basis_changer, local_parameters.point_plane,
                                         global_parameters.point_plane);

  Point3Ops::change_basis(basis_changer, local_parameters.fuge,
                                         global_parameters.fuge);
  */

  /*
  Point3 translation;
  Point3Ops::change_basis(basis_changer, position, translation);
  */

  //global_parameters.fuge        += translation;
  //global_parameters.point_plane += translation;

  /*
  basis.set(0, 0, basis.get(0, 0) * position.x());
  basis.set(0, 1, basis.get(0, 1) * position.y());
  basis.set(0, 2, basis.get(0, 2) * position.z());

  basis.set(1, 0, basis.get(1, 0) * position.x());
  basis.set(1, 1, basis.get(1, 1) * position.y());
  basis.set(1, 2, basis.get(1, 2) * position.z());

  basis.set(2, 0, basis.get(2, 0) * position.x());
  basis.set(2, 1, basis.get(2, 1) * position.y());
  basis.set(2, 2, basis.get(2, 2) * position.z());
*/

  //basis = basis * position;


  //std::cout << position.z() << std::endl;

/*
  std::cout << "Vector normal :"
            << global_parameters.point_plane.x() << " "
            << global_parameters.point_plane.y() << " "
            << global_parameters.point_plane.z() << "\n";
  */
}
