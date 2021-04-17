#ifndef ABSTRACTRASTERISER_H
#define ABSTRACTRASTERISER_H

#include "../buffers/commonbuffers.h"
#include "../spatial/camera.h"

#include "../math/point2.h"
#include "../math/point2d.h"
#include "../math/point3.h"
#include "../planar/triangle.h"
#include "../planar/texture.h"

#include <vector>
#include <algorithm>

class AbstractRasteriser {
protected:
  CommonBuffers& buffers;

   void update_buffers(unsigned x,
                       unsigned y,
                       double z_value,
                       unsigned long t_index) const;

  Texture<unsigned long, 1>* indices_target_surface;
  Texture<double, 1>* z_target_surface;
  const Camera* camera;

  unsigned width;
  unsigned height;

public:  

  AbstractRasteriser() ;
  virtual void rasterise (const Camera& cam,
                          Texture<unsigned long, 1>& i_surface,
                          Texture<double, 1>& z_surface) = 0;
};

#endif // ABSTRACTRASTERISER_H
