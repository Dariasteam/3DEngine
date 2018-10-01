#include "camera.h"

Camera::Camera() {
  fuge = {0, 0, 0};
  plane = {0, 0, 50};
  bounds = {-60, -60, 60, 60};
}
