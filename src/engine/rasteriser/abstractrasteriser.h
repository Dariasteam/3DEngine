#ifndef ABSTRACTRASTERISER_H
#define ABSTRACTRASTERISER_H

#include "../camera.h"
#include "../world.h"
#include "../../frontends/framebufferhandler.h"

#include "../math/point2.h"
#include "../math/point2d.h"
#include "../math/point3.h"
#include "../math/point3d.h"

#include <vector>
#include <algorithm>


class AbstractRasteriser {
protected:
  unsigned screen_size = SCREEN_SIZE;

  World* world;
  Camera* camera;
  FrameBufferHandler* canvas;

  double* z_buff = {new double[SCREEN_SIZE * SCREEN_SIZE]};
  unsigned char* screen_buff_a {new unsigned char [SCREEN_SIZE * SCREEN_SIZE * 3]};
  unsigned char* screen_buff_b {new unsigned char [SCREEN_SIZE * SCREEN_SIZE * 3]};

  inline unsigned toScreenIndex (int x, int y) {
    return y * (SCREEN_SIZE * 3) + (x * 3);
  }

  inline unsigned toDepthIndex (int x, int y) {
    return y * SCREEN_SIZE + x;
  }
public:  

  AbstractRasteriser(World* w, FrameBufferHandler* cv);

  virtual void rasterise (std::vector<Triangle2i>* triangles, unsigned sz) = 0;
};

#endif // ABSTRACTRASTERISER_H
