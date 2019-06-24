#include "camera.h"

Camera::Camera() :
  fuge ({0, 0, 0}),
  vector_plane (Vector3{0, 0, 1}),
  point_plane ({0, 0, 8}) // controls fovy
  {

  double b = 3;    // angle
  bounds = {-b, -b, b, b};

  translation = {0, 0, 0};

  basis = Basis3 ({
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
  });
}
