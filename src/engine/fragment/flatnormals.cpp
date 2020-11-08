#include "flatnormals.h"

void FlatNormals::operator ()(unsigned pixel_index) {
  unsigned char x = 0;
  unsigned char y = 0;

  Point2i p = pixel_index_to_coordenates(pixel_index);

  unsigned t_index = get_triangle_index_at_pixel_index(pixel_index);
  Color888 c = texture_projectors[t_index].get_color_on_uv(p.X, p.Y, normal_map);


  double r = get_triangle_at_pixel_index(pixel_index).normal.X;
  double g = get_triangle_at_pixel_index(pixel_index).normal.Z;

  r += (2.0 * double(c.r) - 255) / 255;
  g += (2.0 * double(c.g) - 255) / 255;

  x += std::floor(128 + 128.0 * r);
  y += std::floor(128 + 128.0 * g);

  CommonBuffers::get().normal_buffer.set(pixel_index * 3 + 0, x);
  CommonBuffers::get().normal_buffer.set(pixel_index * 3 + 2, y);
  CommonBuffers::get().normal_buffer.set(pixel_index * 3 + 2, 255);
}
