#include "lightness.h"

#define BASE_LIGHT 1

void Lightness::operator ()(unsigned pixel_index) {
  const DirectionalLight& light = world->get_light();

  double normal_x = CommonBuffers::get().normal_buffer.get(pixel_index * 3 + 0);
  double normal_y = CommonBuffers::get().normal_buffer.get(pixel_index * 3 + 1);

  // Convert to a value between -1 and 1 again
  normal_x = (2 * normal_x - 255) / 255;
  normal_y = (2 * normal_y - 255) / 255;

  Vector3 normal(normal_x, normal_y, 1);

  // Using dot product as angle
  double angle_to_light = 1 + (normal * light.direction) * light.intensity;

  Color color = light.color;
  color *= angle_to_light;

  double r = CommonBuffers::get().screen_buffer.get(pixel_index * 3 + 0);
  double g = CommonBuffers::get().screen_buffer.get(pixel_index * 3 + 1);
  double b = CommonBuffers::get().screen_buffer.get(pixel_index * 3 + 2);

  unsigned char r_ = std::round(std::min(255.0, std::max(0.0, BASE_LIGHT + color.x() * r)));
  unsigned char g_ = std::round(std::min(255.0, std::max(0.0, BASE_LIGHT + color.y() * g)));
  unsigned char b_ = std::round(std::min(255.0, std::max(0.0, BASE_LIGHT + color.z() * b)));

  CommonBuffers::get().screen_buffer.set(pixel_index * 3 + 0, r_);
  CommonBuffers::get().screen_buffer.set(pixel_index * 3 + 1, g_);
  CommonBuffers::get().screen_buffer.set(pixel_index * 3 + 2, b_);
}
