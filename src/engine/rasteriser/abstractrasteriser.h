#ifndef ABSTRACTRASTERISER_H
#define ABSTRACTRASTERISER_H

#include "../buffers/commonbuffers.h"
#include "../spatial/camera.h"

#include "../math/point2.h"
#include "../math/point3.h"
#include "../planar/triangle.h"
#include "../planar/texture.h"

#include <vector>
#include <algorithm>

class AbstractRasteriser {
protected:
  CommonBuffers& buffers;

   inline void update_buffers(unsigned x,
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
        buffers.is_triangle_occluded[current_t] = true;
      }

      z_target_surface->set(x, y, z_value);
      indices_target_surface->set(x, y, t_index);
    } else if (occlusion) {
      buffers.is_triangle_occluded[t_index] = true;
    }
  }

  TriangleIndexBuffer* indices_target_surface;
  ZBuffer* z_target_surface;
  const Camera* camera;

  unsigned width;
  unsigned height;

public:

  AbstractRasteriser() ;
  virtual ~AbstractRasteriser() {}
  virtual void rasterise (const Camera& cam,
                          TriangleIndexBuffer& i_surface,
                          ZBuffer& z_surface) = 0;
};

#endif // ABSTRACTRASTERISER_H
