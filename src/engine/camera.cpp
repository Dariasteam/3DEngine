#include "camera.h"

Camera::Camera() :
  fuge ({0, 0, 0}),
  vector_plane (Vector3{0, 0, 1}),
  point_plane ({0, 0, 8}) // controls fovy
  {

  // FIXME USE 1
  double b = 3;    // angle
  bounds = {-b, -b, b, b};
}
