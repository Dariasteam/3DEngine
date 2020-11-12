#include "commonbuffers.h"

void CommonBuffers::set_dimension(unsigned w, unsigned h) {
  triangle_index_buffer = Texture<unsigned long, 1> (w, h);
  z_buffer = Texture<double, 1> (w, h);
  normal_buffer = Texture<unsigned char, 3> (w, h);
  screen_buffer = Texture<unsigned char, 3> (w, h);
}

void CommonBuffers::reset_z_buffer() {
  z_buffer.fill(INFINITY_DISTANCE);
}
