#ifndef ABSTRACTRASTERISER_H
#define ABSTRACTRASTERISER_H

#include "../buffers/commonbuffers.h"
#include "../spatial/camera.h"

#include "../math/point2.h"
#include "../math/point2d.h"
#include "../math/point3.h"
#include "../math/point3d.h"
#include "../planar/triangle.h"

#include <vector>
#include <algorithm>

class AbstractRasteriser {
protected:
  //unsigned screen_size = SCREEN_SIZE;
  CommonBuffers& buffers;    

  inline void update_buffers(unsigned x, unsigned y,
                             double z_value, unsigned long t_index) const {
    if (z_value < buffers.z_buffer.get(x, y)) {
      buffers.z_buffer.set(x, y, z_value);
      buffers.triangle_index_buffer.set(x, y, t_index);
    }
  }

  unsigned width;
  unsigned height;
  const Camera* camera;

public:  

  AbstractRasteriser();
  virtual void rasterise (const Camera& cam, unsigned w, unsigned h) = 0;
};

#endif // ABSTRACTRASTERISER_H
