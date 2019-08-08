#ifndef RASTERISERWIREFRAME_H
#define RASTERISERWIREFRAME_H

#include "rasteriser.h"
#include "canvas.h"

class RasteriserWireframe : public Rasteriser {
private:
  inline void fillTopFlatTriangle(const Triangle2& triangle,
                              std::vector<std::vector<Color888>>* screen_buffer);

  inline void fillBottomFlatTriangle(const Triangle2& triangle,
                              std::vector<std::vector<Color888>>* screen_buffer);

  void rasterize_triangle (Triangle2& triangle,
                           std::vector<std::vector<Color888>>* screen_buffer);

public:
  RasteriserWireframe(World* w, Canvas* cv) : Rasteriser (w, cv) {}
};

#endif // RASTERISERWIREFRAME_H
