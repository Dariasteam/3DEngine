#include "commonbuffers.h"

void CommonBuffers::set_dimension(unsigned w, unsigned h) {
  triangle_index_surface = Texture<unsigned long, 1> (w, h);
  l_triangle_index_surface = Texture<unsigned long, 1> (w * 2, h * 2);

  z_buffer = Texture<double, 1> (w, h);

  z_light = Texture<double, 1> (w * 2, h * 2);

  normal_buffer = Texture<unsigned char, 3> (w, h);
  screen_buffer = Texture<unsigned char, 3> (w, h);
}

void CommonBuffers::reset_z_buffer() {
  z_buffer.fill(INFINITY_DISTANCE);
}
