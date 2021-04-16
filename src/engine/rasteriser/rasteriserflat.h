#ifndef RASTERISERFLAT_H
#define RASTERISERFLAT_H

#include "abstractrasteriserCPU.h"

class RasteriserFlat : public AbstractRasteriserCPU {
private:
  inline void fillTopFlatTriangle(const Triangle& triangle, unsigned t_index) const;
  inline void fillBottomFlatTriangle(const Triangle& triangle, unsigned t_index) const;

  void rasterize_triangle (Triangle& triangle, unsigned t_index) const;
public:
  RasteriserFlat() {}
};

#endif // RASTERISERFLAT_H
