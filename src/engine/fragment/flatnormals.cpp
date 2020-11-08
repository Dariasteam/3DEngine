#include "flatnormals.h"

void FlatNormals::operator ()(unsigned pixel_index) {
  unsigned t_index = CommonBuffers::get().triangle_index_buffer.get(pixel_index);

  unsigned char x = 128;
  unsigned char y = 128;
  unsigned char z = 128;

  x += 128 * (*triangles)[t_index].normal.X;
  y += 128 * (*triangles)[t_index].normal.Y;
  z += 128 * (*triangles)[t_index].normal.Z;

  CommonBuffers::get().normal_buffer.set(pixel_index * 3 + 0, y);
  //CommonBuffers::get().normal_buffer.set(pixel_index * 3 + 1, z);
  CommonBuffers::get().normal_buffer.set(pixel_index * 3 + 2, x);
}
