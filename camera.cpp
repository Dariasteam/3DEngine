#include "camera.h"

Camera::Camera() {
  fuge = {0, 0, 0};
  plane = {0, 0, 10};
  bounds = {-60, -60, 60, 60};
}
