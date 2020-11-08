#include "abstractrasteriser.h"

AbstractRasteriser::AbstractRasteriser(World* w) :
  world (w),
  camera (w->get_camera())
{}

void CommonBuffers::set_dimension(unsigned w, unsigned h) {
  triangle_index_buffer = Texture<unsigned long, 1> (w, h);
  z_buffer = Texture<double, 1> (w, h);
  normal_buffer = Texture<unsigned char, 3> (w, h);
  screen_buffer = Texture<unsigned char, 3> (w, h);
}

// We could use a mask insted of cleaning everything
void CommonBuffers::clean() {
  z_buffer.fill(INFINITY_DISTANCE);
  triangle_index_buffer.fill(0);    // FIXME: Can we avoid cleaning this one?
  normal_buffer.fill(0);
  screen_buffer.fill(255);
}
