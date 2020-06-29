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

  std::vector<std::vector<Color888>> screen_buffer_a;
  std::vector<std::vector<Color888>> screen_buffer_b;
  //std::vector<std::vector<double>> z_buff;
  double* z_buff = new double[SCREEN_SIZE * SCREEN_SIZE];

  unsigned char* screen_buff_a;
  unsigned char* screen_buff_b;

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
    canvas (cv),    
    screen_buffer_a (screen_size, std::vector<Color888>(screen_size, {0,0,0})),
    screen_buffer_b (screen_size, std::vector<Color888>(screen_size, {0,0,0}))
    //z_buff (screen_size, std::vector<double>(screen_size, 10000))
  {    
    screen_buff_a = new unsigned char [SCREEN_SIZE * SCREEN_SIZE * 3];
    screen_buff_b = new unsigned char [SCREEN_SIZE * SCREEN_SIZE * 3];
    canvas->set_screen_buffers(&screen_buffer_a, &screen_buffer_b, screen_buff_a, screen_buff_b);
  }

  virtual void rasterise (std::vector<Triangle2i>& triangles) = 0;
};

#endif // ABSTRACTRASTERISER_H
