#ifndef PROJECTOR_H
#define PROJECTOR_H

#include "abstractrasteriser.h"
#include "../../auxiliar/multithreadmanager.h"

#include <vector>
#include <algorithm>

class AbstractRasteriserCPU : public AbstractRasteriser {
private:
protected:
  unsigned char* buff;

  inline void writeColorToCurrentBuffer(const int x,
                                        const int y,
                                        const Color& c) {

    Color888 color (c);
    writeColorToCurrentBuffer(x, y, color);
  }

  inline void writeColorToCurrentBuffer(const int x,
                                        const int y,
                                        const Color888& c) {
    unsigned index = toScreenIndex(x, y);
    buff[index] = c.r;
    buff[index + 1] = c.g;
    buff[index + 2] = c.b;
  }

  inline void writeDepthBuffer(const int x, const int y, double z) {
    unsigned index = toDepthIndex(x, y);
    z_buff[index] = z;
  }

  inline bool triangle_inside_screen (const Triangle2i& triangle);
  inline Triangle2i triangle_to_screen_space (const Triangle2& triangle);

  virtual void rasterize_triangle (Triangle2i& triangle) = 0;

public:
  AbstractRasteriserCPU(World* w, FrameBufferHandler* cv) : AbstractRasteriser (w, cv) {}

  void rasterise (std::vector<Triangle2i>* triangles, unsigned sz);
};

#endif // PROJECTOR_H
