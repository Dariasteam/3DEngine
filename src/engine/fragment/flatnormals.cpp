#include "flatnormals.h"

void FlatNormals::operator ()(unsigned pixel_index) {
  Point2i p = pixel_index_to_coordenates(pixel_index);

  unsigned t_index = get_triangle_index_at_pixel_index(pixel_index);
  Color888 c = texture_projectors[t_index].get_color_on_uv(p.X, p.Y, normal_map);

  double r = get_triangle_at_pixel_index(pixel_index).normal.X;
  double g = get_triangle_at_pixel_index(pixel_index).normal.Z;

  r -= (2.0 * double(c.r) - 255) / 255;
  g -= (2.0 * double(c.g) - 255) / 255;

  r = std::min(1.0, std::max(-1.0, r));
  g = std::min(1.0, std::max(-1.0, g));

  unsigned char x = 128 + std::round(127.0 * r);
  unsigned char y = 128 + std::round(127.0 * g);

  buffers.normal_buffer.set(pixel_index * 3 + 0, x);
  buffers.normal_buffer.set(pixel_index * 3 + 2, y);
  buffers.normal_buffer.set(pixel_index * 3 + 2, 255);
}
