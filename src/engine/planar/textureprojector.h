#ifndef TEXTUREPROJECTOR_H
#define TEXTUREPROJECTOR_H

#include "../../frontends/framebufferhandler.h"
#include "../math/matrix2.h"
#include "texture.h"
#include "triangle.h"
#include "vertex.h"

#include <algorithm>
#include <iostream>

class TextureProjector {
  Texture screen;

  Matrix2 basis_changer;
  Point2i t_origin;
  Texture const* texture;
public:
  TextureProjector();

  void project (const Texture& tex,
                Triangle2i& projected_triangle,
                const UV& uv);

  void generate_uv_projector (const Texture& tex,
                              const Triangle2i& projected_triangle,
                              const UV& uv);

  Color888 get_color_on_uv (unsigned x, unsigned y) {
    Matrix m ({x - t_origin.X, y - t_origin.Y});
    auto m2 = m * basis_changer;

    int x_tex = std::round(m2[0][0]);
    int y_tex = std::round(m2[0][1]);

    return texture->get_color(x_tex, y_tex);
  }

  Point2i get_point_on_uv (unsigned x, unsigned y) {
    Matrix m ({x - t_origin.X, y - t_origin.Y});
    auto m2 = m * basis_changer;

    int x_tex = std::round(m2[0][0]);
    int y_tex = std::round(m2[0][1]);

    return {x_tex, y_tex};
  }

  void fillBottomFlatTriangle (const Triangle2i& triangle, const Texture& tex);
  void fillTopFlatTriangle (const Triangle2i& triangle, const Texture& tex);
  void rasterize_triangle (Triangle2i& triangle, const Texture& tex);
  void paint_to_screen (unsigned x, unsigned y, const Texture& tex);
};

#endif // TEXTUREPROJECTOR_H
