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
  Point2  uv_origin;
public:
  TextureProjector();

  void generate_uv_projector (const Triangle& projected_triangle,
                              const UV& uv);

  Color888 get_color_on_uv (const int x, const int y, const Texture<unsigned char, 3>& tex) const;


  template <typename T, unsigned D>
  inline std::vector<T> get_on_uv (const int x, const int y, const Texture<T, D>& texture) const {
    const auto& point = get_point_on_uv(x, y, texture);
    std::vector<T> aux;

    for (unsigned i = 0; i < texture.depth(); i++)
      aux.push_back(texture.get(point.X, point.Y, i));

    return aux;
  }

  template <typename T, unsigned D>
  inline Point2i get_point_on_uv  (const int x, const int y, const Texture<T, D>& texture) const {
    Matrix m ({{double(x - t_origin.X), double(y - t_origin.Y)}});

    auto m2 = m * basis_changer;

    int x_tex = std::round((uv_origin.X + m2[0][0]) * texture.width());
    int y_tex = std::round((uv_origin.Y + m2[0][1]) * texture.height());

    // FIXME: This should not exist, there should be no errors
    if (x_tex < 0 || x_tex > texture.width() ||
        y_tex < 0 || y_tex > texture.height()) {

      x_tex = std::min({x_tex, texture.width()});
      x_tex = std::max({x_tex, 0});

      y_tex = std::min({y_tex, texture.height()});
      y_tex = std::max({y_tex, 0});
    }

    return {x_tex, y_tex};
  }
};

#endif // TEXTUREPROJECTOR_H
