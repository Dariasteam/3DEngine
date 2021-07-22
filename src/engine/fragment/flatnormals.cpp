#include "flatnormals.h"

void FlatNormals::operator ()(unsigned pixel_index) {
  auto& triangle = get_triangle_at_pixel_index(pixel_index);

  double x = triangle.normal.X;
  double y = triangle.normal.Y;

  unsigned char r = 128 + std::round(127.0 * x);
  unsigned char g = 128 + std::round(127.0 * y);

  buffers.normal_buffer.set(pixel_index * 3 + 0, r);
  buffers.normal_buffer.set(pixel_index * 3 + 1, g);
  //buffers.normal_buffer.set(pixel_index * 3 + 2, 255);
}
