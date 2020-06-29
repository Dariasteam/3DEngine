#ifndef RASTERISERWIREFRAME_H
#define RASTERISERWIREFRAME_H

#include "abstractrasteriserCPU.h"
#include "canvas.h"

class RasteriserWireframe : public AbstractRasteriserCPU {
private:
  inline void fillTopFlatTriangle(const Triangle2i& triangle,
                              std::vector<std::vector<Color888>>* screen_buffer);

  inline void fillBottomFlatTriangle(const Triangle2i& triangle,
                              std::vector<std::vector<Color888>>* screen_buffer);

  void rasterize_triangle (Triangle2i& triangle,
                           std::vector<std::vector<Color888>>* screen_buffer);

public:
  RasteriserWireframe(World* w, Canvas* cv) : AbstractRasteriserCPU (w, cv) {}
};

#endif // RASTERISERWIREFRAME_H
