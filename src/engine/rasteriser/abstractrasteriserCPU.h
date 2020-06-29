#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "abstractrasteriser.h"
#include "../../auxiliar/multithreadmanager.h"

#include <vector>
#include <algorithm>

class AbstractRasteriserCPU : public AbstractRasteriser {
protected:

  bool isCurrentBufferA;

  inline void writeToCurrentBuffer(const int x, const int y, const Color& c) {
    unsigned index = toLinearIndex(x, y);
    Color888 color (c);
    if (isCurrentBufferA) {
      screen_buff_a[index] = color.r;
      screen_buff_a[index] = color.g;
      screen_buff_a[index] = color.b;
    } else {
      screen_buff_b[index] = color.r;
      screen_buff_b[index] = color.g;
      screen_buff_b[index] = color.b;
    }
  }

  inline bool triangle_inside_screen (const Triangle2i& triangle);
  inline Triangle2i triangle_to_screen_space (const Triangle2& triangle);

  virtual void rasterize_triangle (Triangle2i& triangle,
                                   std::vector<std::vector<Color888>>* screen_buffer) = 0;

public:
  AbstractRasteriserCPU(World* w, Canvas* cv) : AbstractRasteriser (w, cv) {}

  void rasterise (std::vector<Triangle2i>& triangles);
};

#endif // PROJECTOR_H
