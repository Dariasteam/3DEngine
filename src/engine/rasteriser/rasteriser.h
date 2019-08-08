#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "../camera.h"
#include "../world.h"
#include "../../auxiliar/multithreadmanager.h"
#include "../../frontends/Qt/canvas.h"

#include <vector>
#include <algorithm>

class Rasteriser {
protected:
  unsigned screen_size = SCREEN_SIZE;

  World* world;
  Camera* camera;
  Canvas* canvas;

  std::vector<std::vector<Color888>> screen_buffer_a;
  std::vector<std::vector<Color888>> screen_buffer_b;
  std::vector<std::vector<double>> z_buffer;

  inline bool triangle_inside_screen (const Triangle2i& triangle);
  inline Triangle2i triangle_to_screen_space (const Triangle2& triangle);

  virtual void rasterize_triangle (Triangle2i& triangle,
                                   std::vector<std::vector<Color888>>* screen_buffer) = 0;

public:
  Rasteriser(World* w, Canvas* cv) :
    world (w),
    camera (w->get_camera()),
    canvas (cv),

    screen_buffer_a (screen_size, std::vector<Color888>(screen_size, {0,0,0})),
    screen_buffer_b (screen_size, std::vector<Color888>(screen_size, {0,0,0})),
    z_buffer (screen_size, std::vector<double>(screen_size, 10000))
  {
    canvas->set_screen_buffers(&screen_buffer_a, &screen_buffer_b);
  }

  void rasterise (std::vector<Triangle2i>& triangles);
};

#endif // PROJECTOR_H
