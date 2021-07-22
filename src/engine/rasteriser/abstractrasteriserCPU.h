#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "abstractrasteriser.h"
#include "../../auxiliar/multithreadmanager.h"

#include <vector>
#include <algorithm>

class AbstractRasteriserCPU : public AbstractRasteriser {
private:
protected:
  virtual void rasterize_triangle (Triangle& triangle, unsigned t_index) const = 0;
  void triangle_to_surface_space (Triangle& triangle) const;
public:
  AbstractRasteriserCPU()
  {}

  void rasterise (const Camera& cam,
                  TriangleIndexBuffer& i_surface,
                  ZBuffer& z_surface) ;
};

#endif // PROJECTOR_H
