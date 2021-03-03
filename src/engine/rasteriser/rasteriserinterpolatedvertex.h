#ifndef RASTERISERINTERPOLATEDVERTEX_H
#define RASTERISERINTERPOLATEDVERTEX_H

#include "abstractrasteriserCPU.h"

class RasteriserInterpolatedVertex : public AbstractRasteriserCPU {
private:
  inline void fillTopFlatTriangle(const Triangle& triangle, unsigned t_index) const;
  inline void fillBottomFlatTriangle(const Triangle& triangle, unsigned t_index) const;

  void rasterize_triangle (Triangle& triangle, unsigned t_index) const;
public:
  RasteriserInterpolatedVertex() {}
};

#endif // RASTERISERINTERPOLATEDVERTEX_H
