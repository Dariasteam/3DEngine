#ifndef ABSTRACTRASTERISER_H
#define ABSTRACTRASTERISER_H

#include "../camera.h"
#include "../world.h"
//#include "../../frontends/framebufferhandler.h"
#include "../planar/texture.h"

#include "../math/point2.h"
#include "../math/point2d.h"
#include "../math/point3.h"
#include "../math/point3d.h"
#include "../planar/triangle.h"

#include <vector>
#include <algorithm>

//#define SCREEN_SIZE 1000

class AbstractRasteriser {
protected:
  //unsigned screen_size = SCREEN_SIZE;

  // FIXME delete these and use the info of the textures
  unsigned width;
  unsigned height;

  World* world;
  Camera* camera;

  Texture<unsigned long, 1> triangle_buffer;
  Texture<double, 1> z_buffer;

  //FrameBufferHandler* canvas;
/*
  double* z_buff = {new double[SCREEN_SIZE * SCREEN_SIZE]};
  unsigned char* screen_buff_a {new unsigned char [SCREEN_SIZE * SCREEN_SIZE * 3]};
  unsigned char* screen_buff_b {new unsigned char [SCREEN_SIZE * SCREEN_SIZE * 3]};
*/
  /*
  inline unsigned toScreenIndex (int x, int y) {
    return y * (SCREEN_SIZE * 3) + (x * 3);
  }

  inline unsigned toDepthIndex (int x, int y) {
    return y * SCREEN_SIZE + x;
  }
  */

  inline void update_buffers(unsigned x, unsigned y,
                             double z_value, unsigned long t_index) {
    if (z_value < z_buffer.get(x, y)) {
      z_buffer.set(x, y, z_value);
      triangle_buffer.set(x, y, t_index);
    }
  }

public:  

  AbstractRasteriser(World* w);

  inline Texture<unsigned long, 1>& get_triangle_buffer () {
    return triangle_buffer;
  }

  inline Texture<double, 1>& get_z_buffer () {
    return z_buffer;
  }

  void set_screen_dimensions (unsigned w, unsigned h) {

    triangle_buffer = Texture<unsigned long, 1> (w, h);
    z_buffer = Texture<double, 1> (w, h);

    height = h;
    width = w;
  };

  virtual void rasterise (std::vector<Triangle2i>* triangles,
                          unsigned sz) = 0;
};

#endif // ABSTRACTRASTERISER_H
