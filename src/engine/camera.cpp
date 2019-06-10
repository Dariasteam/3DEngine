#include "camera.h"

Camera::Camera() {
  local_parameters.fuge = {0, 0, 0};
  local_parameters.vector_plane = {0, 0, 1};
  local_parameters.point_plane = {0, 0, 8}; // controls fovy

  global_parameters.fuge         = local_parameters.fuge;
  global_parameters.vector_plane = local_parameters.vector_plane;
  global_parameters.point_plane  = local_parameters.point_plane;

  double b = 3;    // angle
  bounds = {-b, -b, b, b};

  translation = {0, 0, 0};

  basis = Basis3 ({
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  });
}
