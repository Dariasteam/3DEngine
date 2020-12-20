#include "normalmapping.h"

void NormalMapping::operator ()(unsigned pixel_index) {
  auto& triangle = get_triangle_at_pixel_index(pixel_index);

  Point2i p = pixel_index_to_screen_coordenates(pixel_index);

  unsigned t_index = get_triangle_index_at_pixel_index(pixel_index);
  Color888 c = texture_projectors[t_index].get_color_on_uv(p.X, p.Y, normal_map);

  double x = triangle.normal.X;
  double y = triangle.normal.Y;

  x -= (2.0 * double(c.r) - 255) / 255;
  y -= (2.0 * double(c.g) - 255) / 255;

  x = std::min(1.0, std::max(-1.0, x));
  y = std::min(1.0, std::max(-1.0, y));

  unsigned char r = 128 + std::round(127.0 * x);
  unsigned char g = 128 + std::round(127.0 * y);

  buffers.normal_buffer.set(pixel_index * 3 + 0, r);
  buffers.normal_buffer.set(pixel_index * 3 + 1, g);
  buffers.normal_buffer.set(pixel_index * 3 + 2, 255);
}
