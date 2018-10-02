#include "camera.h"

Camera::Camera() {
  fuge = {0, 0, 0};
  plane = {0, 0, 1};

  bounds = {-30, -30, 30, 30};

  position = {0, 0, 0};

  basis = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  };

}

#include <iostream>

void Camera::apply_matrix(const Matrix3 &matrix) {
  /*
  projection_plane.v1 = matrix.be_multiplicated_by(projection_plane.v1);
  projection_plane.v2 = matrix.be_multiplicated_by(projection_plane.v2);
  projection_plane.p  = matrix.be_multiplicated_by(projection_plane.p);
  */

  // plane = matrix.be_multiplicated_by(plane);

  basis.a = matrix.be_multiplicated_by(basis.a);
  basis.b = matrix.be_multiplicated_by(basis.b);
  basis.c = matrix.be_multiplicated_by(basis.c);  

/*
  std::cout << projection_plane.v2.x << ", " <<
               projection_plane.v2.y << ", " <<
               projection_plane.v2.z << std::endl;

  std::cout << plane.x << ", " <<
               plane.y << ", " <<
               plane.z << std::endl;
               */
}
