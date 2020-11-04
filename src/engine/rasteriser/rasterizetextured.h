#ifndef RASTERIZETEXTURED_H
#define RASTERIZETEXTURED_H

#include "abstractrasteriserCPU.h"

class RasterizeTextured: public AbstractRasteriserCPU {
private:
  inline void fillTopFlatTriangle(const Triangle2i& triangle,
                              std::vector<std::vector<Color888>>* screen_buffer);

  inline void fillBottomFlatTriangle(const Triangle2i& triangle,
                              std::vector<std::vector<Color888>>* screen_buffer);

  void rasterize_triangle (Triangle2i& triangle,
                           std::vector<std::vector<Color888>>* screen_buffer);
public:
  RasterizeTextured(World* w, FrameBufferHandler* cv) : AbstractRasteriserCPU (w, cv) {}
};

#endif // RASTERIZETEXTURED_H
