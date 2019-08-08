#ifndef RASTERISERFLAT_H
#define RASTERISERFLAT_H

#include "rasteriser.h"
#include "canvas.h"

class RasteriserFlat : public Rasteriser {
private:
  inline void fillTopFlatTriangle(const Triangle2i& triangle,
                              std::vector<std::vector<Color888>>* screen_buffer);

  inline void fillBottomFlatTriangle(const Triangle2i& triangle,
                              std::vector<std::vector<Color888>>* screen_buffer);

  void rasterize_triangle (Triangle2i& triangle,
                           std::vector<std::vector<Color888>>* screen_buffer);
public:
  RasteriserFlat(World* w, Canvas* cv) : Rasteriser (w, cv) {}
};

#endif // RASTERISERFLAT_H
