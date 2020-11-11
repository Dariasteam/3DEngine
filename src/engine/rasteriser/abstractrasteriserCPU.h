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
  void triangle_to_screen_space (Triangle& t) const;

public:
  AbstractRasteriserCPU(World* w) : AbstractRasteriser (w) {}

  void rasterise ();
};

#endif // PROJECTOR_H
