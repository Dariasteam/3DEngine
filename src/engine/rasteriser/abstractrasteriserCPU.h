#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "abstractrasteriser.h"
#include "../../auxiliar/multithreadmanager.h"

#include <vector>
#include <algorithm>

class AbstractRasteriserCPU : public AbstractRasteriser {
private:
protected:
  virtual void rasterize_triangle (const Triangle2i& triangle, unsigned t_index) const = 0;

public:
  AbstractRasteriserCPU(World* w) : AbstractRasteriser (w) {}

  void rasterise (std::vector<Triangle2i>* triangles, unsigned sz);
};

#endif // PROJECTOR_H
