#ifndef RASTERISERFLAT_H
#define RASTERISERFLAT_H

#include "abstractrasteriserCPU.h"

class RasteriserFlat : public AbstractRasteriserCPU {
private:
  inline void fillTopFlatTriangle(const Triangle2i& triangle, unsigned t_index) const;
  inline void fillBottomFlatTriangle(const Triangle2i& triangle, unsigned t_index) const;

  void rasterize_triangle (const Triangle2i& triangle, unsigned t_index) const;
public:
  RasteriserFlat(World* w) : AbstractRasteriserCPU (w) {}
};

#endif // RASTERISERFLAT_H
