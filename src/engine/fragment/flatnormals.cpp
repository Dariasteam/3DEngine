#include "flatnormals.h"

void FlatNormals::operator ()(unsigned pixel_index) {
  unsigned char x = 128;
  unsigned char y = 128;
  unsigned char z = 128;

  x += 128 * get_triangle_at_pixel_index(pixel_index).normal.X;
  y += 128 * get_triangle_at_pixel_index(pixel_index).normal.Y;
  z += 128 * get_triangle_at_pixel_index(pixel_index).normal.Z;

  CommonBuffers::get().normal_buffer.set(pixel_index * 3 + 0, x);
  CommonBuffers::get().normal_buffer.set(pixel_index * 3 + 1, y);
  CommonBuffers::get().normal_buffer.set(pixel_index * 3 + 2, z);
}
