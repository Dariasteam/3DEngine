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
