#include "lightness.h"

void Lightness::operator ()(unsigned pixel_index) {
  const DirectionalLight& light = world->get_light();

  double normal_x = 128 - CommonBuffers::get().normal_buffer.get(pixel_index * 3 + 0);
  double normal_y = 128 - CommonBuffers::get().normal_buffer.get(pixel_index * 3 + 1);
  double normal_z = 128 - CommonBuffers::get().normal_buffer.get(pixel_index * 3 + 2);

  normal_x /= 255;
  normal_y /= 255;
  normal_z /= 255;

  Vector3 normal(normal_x, normal_y, normal_z);

  // Using dot product as angle
  double angle_to_light = 1 + (normal * light.direction);

  Color color = light.color;

  color *= angle_to_light * light.intensity;

  unsigned char r = CommonBuffers::get().screen_buffer.get(pixel_index * 3 + 0);
  unsigned char g = CommonBuffers::get().screen_buffer.get(pixel_index * 3 + 1);
  unsigned char b = CommonBuffers::get().screen_buffer.get(pixel_index * 3 + 2);


  color.set_x(color.x() * double(r) / 255);
  color.set_y(color.y() * double(g) / 255);
  color.set_z(color.z() * double(b) / 255);

  CommonBuffers::get().screen_buffer.set(pixel_index * 3 + 0, color.X);
  CommonBuffers::get().screen_buffer.set(pixel_index * 3 + 1, color.Y);
  CommonBuffers::get().screen_buffer.set(pixel_index * 3 + 2, color.Z);
}
