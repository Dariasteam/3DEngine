#ifndef ABSTRACTRASTERISER_H
#define ABSTRACTRASTERISER_H

#include "../camera.h"
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

  void set_dimension (unsigned w, unsigned h) {
    triangle_index_buffer = Texture<unsigned long, 1> (w, h);
    z_buffer = Texture<double, 1> (w, h);
    normal_buffer = Texture<unsigned char, 3> (w, h);
  };

  inline unsigned get_height() { return triangle_index_buffer.get_height(); }
  inline unsigned get_width() { return triangle_index_buffer.get_width(); }

};


class AbstractRasteriser {
protected:
  //unsigned screen_size = SCREEN_SIZE;

  World* world;
  Camera* camera;



  inline void update_buffers(unsigned x, unsigned y,
                             double z_value, unsigned long t_index) {        

    if (z_value < CommonBuffers::get().z_buffer.get(x, y)) {
      CommonBuffers::get().z_buffer.set(x, y, z_value);
      CommonBuffers::get().triangle_index_buffer.set(x, y, t_index);
    }
  }

public:  

  AbstractRasteriser(World* w);
  virtual void rasterise (std::vector<Triangle2i>* triangles,
                          unsigned sz) = 0;
};

#endif // ABSTRACTRASTERISER_H
