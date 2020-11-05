#ifndef TEXTUREPROJECTOR_H
#define TEXTUREPROJECTOR_H

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

  inline Color888 get_color_on_uv (int x, int y) {
    const auto& point = get_point_on_uv(x, y);
    return texture->get_color(point.X, point.Y);
  }

  inline Point2i get_point_on_uv (int x, int y) {
    Matrix m ({{double(x - t_origin.X), double(y - t_origin.Y)}});

    auto m2 = m * basis_changer;

    int x_tex = std::round(m2[0][0]);
    int y_tex = std::round(m2[0][1]);

    if (x_tex < 0 || x_tex > 600 || y_tex < 0 || y_tex > 300) {
      return {0, 0};
    }

    return {x_tex, y_tex};
  }

  void fillBottomFlatTriangle (const Triangle2i& triangle, const Texture& tex);
  void fillTopFlatTriangle (const Triangle2i& triangle, const Texture& tex);
  void rasterize_triangle (Triangle2i& triangle, const Texture& tex);
  void paint_to_screen (unsigned x, unsigned y, const Texture& tex);
};

#endif // TEXTUREPROJECTOR_H
