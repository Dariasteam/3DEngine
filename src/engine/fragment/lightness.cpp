#include "lightness.h"

#define BASE_LIGHT 1

void Lightness::operator ()(unsigned pixel_index) {
  const DirectionalLight& light = World::get().get_light();

  double normal_x = buffers.normal_buffer.get(pixel_index * 3 + 0);
  double normal_y = buffers.normal_buffer.get(pixel_index * 3 + 1);

  // Convert to a value between -1 and 1 again
  normal_x = (2 * normal_x - 255) / 255;
  normal_y = (2 * normal_y - 255) / 255;

  Vector3 normal(normal_x, normal_y, 1);

  // Using dot product as angle
  double angle_to_light = 1 + (normal * light.get_direction()) * light.get_intensity();

  Color color = light.get_color();
  color *= angle_to_light;

  double r = buffers.screen_buffer.get(pixel_index * 3 + 0);
  double g = buffers.screen_buffer.get(pixel_index * 3 + 1);
  double b = buffers.screen_buffer.get(pixel_index * 3 + 2);

  unsigned char r_ = std::round(std::min(255.0, std::max(0.0, BASE_LIGHT + color.x() * r)));
  unsigned char g_ = std::round(std::min(255.0, std::max(0.0, BASE_LIGHT + color.y() * g)));
  unsigned char b_ = std::round(std::min(255.0, std::max(0.0, BASE_LIGHT + color.z() * b)));

  buffers.screen_buffer.set(pixel_index * 3 + 0, r_);
  buffers.screen_buffer.set(pixel_index * 3 + 1, g_);
  buffers.screen_buffer.set(pixel_index * 3 + 2, b_);
}
