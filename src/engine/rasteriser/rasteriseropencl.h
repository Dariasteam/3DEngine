#ifndef RASTERISEROPENCL_H
#define RASTERISEROPENCL_H

#include "abstractrasteriserGPU.h"
#include "canvas.h"
#include <CL/cl.hpp>
#include <cstdio>

class RasteriserOpencl : public AbstractRasteriserGPU {
public:
  RasteriserOpencl(World* w, Canvas* cv) : AbstractRasteriserGPU (w, cv) {
    load_kernel("../../src/engine/rasteriser/kernels/k1.cl", "rasterize_triangle");
  }

  void rasterise (std::vector<Triangle2i>& triangles);
};

#endif // RASTERISEROPENCL_H
