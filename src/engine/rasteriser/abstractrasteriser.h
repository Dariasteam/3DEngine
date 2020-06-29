#ifndef ABSTRACTRASTERISER_H
#define ABSTRACTRASTERISER_H

#include "../camera.h"
#include "../world.h"
#include "../../frontends/Qt/canvas.h"

#include <vector>
#include <algorithm>


class AbstractRasteriser {
protected:
  unsigned screen_size = SCREEN_SIZE;

  World* world;
  Camera* camera;
  Canvas* canvas;

  double* z_buff = new double[SCREEN_SIZE * SCREEN_SIZE];

  unsigned char* screen_buff_a = new unsigned char [SCREEN_SIZE * SCREEN_SIZE * 3];
  unsigned char* screen_buff_b = new unsigned char [SCREEN_SIZE * SCREEN_SIZE * 3];

  inline unsigned toScreenIndex (int x, int y) {
    return y * (SCREEN_SIZE * 3) + (x * 3);
  }

  inline unsigned toDepthIndex (int x, int y) {
    return y * SCREEN_SIZE + x;
  }
public:  

  AbstractRasteriser(World* w, Canvas* cv) :
    world (w),
    camera (w->get_camera()),
    canvas (cv)
  {    
    canvas->set_screen_buffers(screen_buff_a, screen_buff_b);
  }

  virtual void rasterise (std::vector<Triangle2i>& triangles) = 0;
};

#endif // ABSTRACTRASTERISER_H
