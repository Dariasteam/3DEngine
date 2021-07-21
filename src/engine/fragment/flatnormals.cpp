#include "flatnormals.h"

void FlatNormals::operator ()(unsigned pixel_index) {
  auto& triangle = get_triangle_at_pixel_index(pixel_index);

  unsigned char r = 127 + triangle.normal.X;
  unsigned char g = 127 + triangle.normal.Y;

  buffers.normal_buffer.set(pixel_index * 3 + 0, r);
  buffers.normal_buffer.set(pixel_index * 3 + 1, g);
  buffers.normal_buffer.set(pixel_index * 3 + 2, 255);
}
