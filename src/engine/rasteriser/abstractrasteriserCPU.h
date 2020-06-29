#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "abstractrasteriser.h"
#include "../../auxiliar/multithreadmanager.h"

#include <vector>
#include <algorithm>

class AbstractRasteriserCPU : public AbstractRasteriser {
protected:

  inline void writeColorToCurrentBuffer(const int x, const int y, const Color& c) {
    unsigned index = toScreenIndex(x, y);
    Color888 color (c);
    if (!canvas->reading_from_buffer_a()) {
      screen_buff_a[index] = color.r;
      screen_buff_a[index + 1] = color.g;
      screen_buff_a[index + 2] = color.b;
    } else {
      screen_buff_b[index] = color.r;
      screen_buff_b[index + 1] = color.g;
      screen_buff_b[index + 2] = color.b;
    }
  }

  inline void writeDepthBuffer(const int x, const int y, double z) {
    unsigned index = toDepthIndex(x, y);
    z_buff[index] = z;
  }

  inline bool triangle_inside_screen (const Triangle2i& triangle);
  inline Triangle2i triangle_to_screen_space (const Triangle2& triangle);

  virtual void rasterize_triangle (Triangle2i& triangle) = 0;

public:
  AbstractRasteriserCPU(World* w, Canvas* cv) : AbstractRasteriser (w, cv) {}

  void rasterise (std::vector<Triangle2i>& triangles);
};

#endif // PROJECTOR_H
