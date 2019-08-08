#ifndef RASTERISERINTERPOLATEDVERTEX_H
#define RASTERISERINTERPOLATEDVERTEX_H

#include "rasteriser.h"
#include "canvas.h"

class RasteriserInterpolatedVertex : public Rasteriser{
private:

  inline void fillTopFlatTriangle(const Triangle2i& triangle,
                              std::vector<std::vector<Color888>>* screen_buffer);

  inline void fillBottomFlatTriangle(const Triangle2i& triangle,
                              std::vector<std::vector<Color888>>* screen_buffer);

  void rasterize_triangle (Triangle2i& triangle,
                           std::vector<std::vector<Color888>>* screen_buffer);

public:
  RasteriserInterpolatedVertex(World* w, Canvas* cv) : Rasteriser (w, cv) {}
};

#endif // RASTERISERINTERPOLATEDVERTEX_H
