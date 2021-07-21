#include "flatnormals.h"

void FlatNormals::operator ()(unsigned pixel_index) {
  auto& triangle = get_triangle_at_pixel_index(pixel_index);

  unsigned char r = triangle.normal.X - 127;
  unsigned char g = triangle.normal.Y - 127;

  buffers.normal_buffer.set(pixel_index * 3 + 0, r);
  buffers.normal_buffer.set(pixel_index * 3 + 1, g);
  buffers.normal_buffer.set(pixel_index * 3 + 2, 255);
}
