#ifndef ABSTRACTRASTERISER_H
#define ABSTRACTRASTERISER_H

#include "../spatial/camera.h"
#include "../world.h"
#include "../planar/texture.h"

#include "../math/point2.h"
#include "../math/point2d.h"
#include "../math/point3.h"
#include "../math/point3d.h"
#include "../planar/triangle.h"

#include <vector>
#include <algorithm>


class CommonBuffers {
private:
  CommonBuffers () {}
public:
  inline static CommonBuffers& get () {
    static CommonBuffers instance;
    return instance;
  }

  Texture<unsigned long, 1> triangle_index_buffer;
  Texture<double, 1> z_buffer;
  Texture<unsigned char, 3> normal_buffer;
  Texture<unsigned char, 3> screen_buffer;

  void set_dimension (unsigned w, unsigned h);;

  inline unsigned get_height() { return triangle_index_buffer.get_height(); }
  inline unsigned get_width() { return triangle_index_buffer.get_width(); }

  std::vector<Triangle> triangles;
  unsigned triangles_size;  

  void clean();
};


class AbstractRasteriser {
protected:
  //unsigned screen_size = SCREEN_SIZE;
  CommonBuffers& buffers;
  World* world;
  PerspectiveCamera* camera;

  inline void update_buffers(unsigned x, unsigned y,
                             double z_value, unsigned long t_index) const {
    if (z_value < buffers.z_buffer.get(x, y)) {
      buffers.z_buffer.set(x, y, z_value);
      buffers.triangle_index_buffer.set(x, y, t_index);
    }
  }

public:  

  AbstractRasteriser(World* w);
  virtual void rasterise () = 0;
};

#endif // ABSTRACTRASTERISER_H
