#include "commonbuffers.h"

void CommonBuffers::set_dimension(unsigned w, unsigned h) {
  triangle_index_surface = TriangleIndexBuffer   (w, h);
  l_triangle_index_surface = TriangleIndexBuffer (w * 2, h * 2);

  z_buffer = ZBuffer (w, h);
  z_light = ZBuffer (w * 2, h * 2);

  normal_buffer = NormalBuffer (w, h);
  screen_buffer = RGBTexture (w, h);
}

void CommonBuffers::reset_z_buffer() {
  z_buffer.fill(INFINITY_DISTANCE);
}
