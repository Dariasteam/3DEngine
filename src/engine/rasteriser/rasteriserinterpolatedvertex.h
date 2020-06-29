#ifndef RASTERISERINTERPOLATEDVERTEX_H
#define RASTERISERINTERPOLATEDVERTEX_H

#include "abstractrasteriserCPU.h"
#include "canvas.h"

class RasteriserInterpolatedVertex : public AbstractRasteriserCPU{
private:

  inline void fillTopFlatTriangle(const Triangle2i& triangle,
                              std::vector<std::vector<Color888>>* screen_buffer);

  inline void fillBottomFlatTriangle(const Triangle2i& triangle,
                              std::vector<std::vector<Color888>>* screen_buffer);

  inline void paintLine (const Triangle2i& triangle,
                         const int min_x,
                         const int max_x,
                         const int y,
                         const Color& gradient,
                         const Color& color,
                         std::vector<std::vector<Color888>>* screen_buffer);

  void rasterize_triangle (Triangle2i& triangle,
                           std::vector<std::vector<Color888>>* screen_buffer);

public:
  RasteriserInterpolatedVertex(World* w, Canvas* cv) : AbstractRasteriserCPU (w, cv) {}
};

#endif // RASTERISERINTERPOLATEDVERTEX_H
