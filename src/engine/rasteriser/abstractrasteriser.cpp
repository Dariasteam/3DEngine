#include "abstractrasteriser.h"

 void AbstractRasteriser::update_buffers(unsigned x,
                                         unsigned y,
                                         double z_value,
                                         unsigned long t_index) const {

  double current_z = z_target_surface->get(x, y);

  bool occlusion = false;

  if (fabs(z_value - current_z) > .35 &&
    current_z != INFINITY_DISTANCE) {
    occlusion = true;
  }

  if (z_value < current_z) {
    if (occlusion) {
      unsigned long current_t = indices_target_surface->get(x, y);
      buffers.is_triangle_ocluded[current_t] = true;
    }

    z_target_surface->set(x, y, z_value);
    indices_target_surface->set(x, y, t_index);
  } else if (occlusion) {
    buffers.is_triangle_ocluded[t_index] = true;
  }
}

AbstractRasteriser::AbstractRasteriser() :
  buffers(CommonBuffers::get())
{}
