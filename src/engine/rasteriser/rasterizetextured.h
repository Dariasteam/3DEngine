#ifndef RASTERIZETEXTURED_H
#define RASTERIZETEXTURED_H

#include "abstractrasteriserCPU.h"
#include "../planar/textureprojector.h"

class RasterizeTextured: public AbstractRasteriserCPU {
private:
  TextureProjector projector;
  Texture tex;

  inline void fillTopFlatTriangle(const Triangle2i& triangle);
  inline void fillBottomFlatTriangle(const Triangle2i& triangle);

  void rasterize_triangle (Triangle2i& triangle);
public:
  RasterizeTextured(World* w, FrameBufferHandler* cv) :
    AbstractRasteriserCPU (w, cv)
  {
    tex.load ("texture.ppm");
  }
};

#endif // RASTERIZETEXTURED_H
