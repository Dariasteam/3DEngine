#ifndef RASTERISERINTERPOLATEDVERTEX_H
#define RASTERISERINTERPOLATEDVERTEX_H

#include "abstractrasteriserCPU.h"
#include "canvas.h"

class RasteriserInterpolatedVertex : public AbstractRasteriserCPU{
private:

  inline void fillTopFlatTriangle(const Triangle2i& triangle);

  inline void fillBottomFlatTriangle(const Triangle2i& triangle);

  inline void paintLine (const Triangle2i& triangle,
                         const int min_x,
                         const int max_x,
                         const int y,
                         const Color& gradient,
                         const Color& color);

  void rasterize_triangle (Triangle2i& triangle);

public:
  RasteriserInterpolatedVertex(World* w, Canvas* cv) : AbstractRasteriserCPU (w, cv) {}
};

#endif // RASTERISERINTERPOLATEDVERTEX_H
