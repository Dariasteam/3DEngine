#ifndef TEXTUREPROJECTOR_H
#define TEXTUREPROJECTOR_H

#include "texture.h"
#include "../math/matrix2.h"
#include "triangle.h"
#include "vertex.h"

#include <algorithm>
#include <iostream>

struct UV {
  Point2 p;
  Vector2 u;
  Vector2 v;
};

class TextureProjector {
  Texture screen;
  Matrix2 basis_changer;
  Point2i t_origin;
public:
  TextureProjector();

  void project (const Texture& tex,
                Triangle2i& projected_triangle,
                const UV& uv);

  void write() const;    

  void fillBottomFlatTriangle (const Triangle2i& triangle, const Texture& tex);
  void fillTopFlatTriangle (const Triangle2i& triangle, const Texture& tex);
  void rasterize_triangle (Triangle2i& triangle, const Texture& tex);
  void paint_to_screen (unsigned x, unsigned y, const Texture& tex);
};

#endif // TEXTUREPROJECTOR_H
