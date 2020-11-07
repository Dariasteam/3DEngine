#ifndef RASTERISERFLAT_H
#define RASTERISERFLAT_H

#include "abstractrasteriserCPU.h"

class RasteriserFlat : public AbstractRasteriserCPU {
private:
  inline void fillTopFlatTriangle(const Triangle2i& triangle, unsigned t_index);
  inline void fillBottomFlatTriangle(const Triangle2i& triangle, unsigned t_index);

  void rasterize_triangle (Triangle2i& triangle, unsigned t_index);
public:
  RasteriserFlat(World* w) : AbstractRasteriserCPU (w) {}
};

#endif // RASTERISERFLAT_H
