#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "abstractrasteriser.h"
#include "../../auxiliar/multithreadmanager.h"

#include <vector>
#include <algorithm>

#define INFINITY_DISTANCE 1000000000

class AbstractRasteriserCPU : public AbstractRasteriser {
private:
protected:
  virtual void rasterize_triangle (Triangle2i& triangle, unsigned t_index) = 0;

public:
  AbstractRasteriserCPU(World* w) : AbstractRasteriser (w) {}

  void rasterise (std::vector<Triangle2i>* triangles, unsigned sz);
};

#endif // PROJECTOR_H
