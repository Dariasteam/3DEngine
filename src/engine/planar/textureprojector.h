#ifndef TEXTUREPROJECTOR_H
#define TEXTUREPROJECTOR_H

#include "../math/matrix2.h"
#include "texture.h"
#include "triangle.h"
#include "vertex.h"

#include <algorithm>
#include <iostream>

class TextureProjector {
private:
  Matrix2 basis_changer;

  Point2i t_origin;
  Point2  v_origin;

  Texture const* texture;
public:
  TextureProjector();

  void project (const Texture& tex,
                Triangle2i& projected_triangle,
                const UV& uv);

  void generate_uv_projector (const Texture& tex,
                              const Triangle2i& projected_triangle,
                              const UV& uv);

  Color888 get_color_on_uv (int x, int y);
  Point2i get_point_on_uv (int x, int y);
};

#endif // TEXTUREPROJECTOR_H
