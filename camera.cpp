#include "camera.h"

Camera::Camera() {
  fuge = {0, 0, 0};
  plane = {0, 0, 20};
  bounds = {-10, -10, 10, 10};
  position = {0, 0, 100};
}
