#include "camera.h"

Camera::Camera() {
  fuge = {0, 0, 0};
  plane = {0, 0, 20};
  bounds = {-20, -20, 20, 20};
}
