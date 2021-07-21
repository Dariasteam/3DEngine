#ifndef TEXTUREPROJECTOR_H
#define TEXTUREPROJECTOR_H

#include "../math/matrix2.h"
#include "../math/point2i.h"
#include "texture.h"
#include "triangle.h"
#include "rect.h"
#include "../planar/color888.h"

#include <algorithm>
#include <iostream>

class TextureProjector {
private:
  Matrix2 basis_changer;

  P2i t_origin;
  Point2 uv_origin;
public:
  TextureProjector();

  void generate_uv_projector (const Triangle& projected_triangle,
                              const UV& uv);

  Color888 get_color_on_uv (const unsigned x,
                            const unsigned y,
                            const Texture<unsigned char, 3>& tex) const;


  template <typename T, unsigned D>
  inline std::vector<T> get_on_uv (const unsigned x,
                                   const unsigned y,
                                   const Texture<T, D>& texture) const {
    const auto& point = get_point_on_uv(x, y, texture);
    std::vector<T> aux;

    for (unsigned i = 0; i < texture.depth(); i++)
      aux.push_back(texture.get(point.X, point.Y, i));

    return aux;
  }

  template <typename T, unsigned D>
  inline P2i get_point_on_uv  (const unsigned x,
                               const unsigned y,
                               const Texture<T, D>& texture) const {

    Matrix V ({{double(x) - t_origin.X, double(y) - t_origin.Y}});

    auto m2 = V * basis_changer;

    int x_tex = std::round((uv_origin.X + m2[0][0]) * texture.width());
    int y_tex = std::round((uv_origin.Y + m2[0][1]) * texture.height());

    // FIXME: This should not exist, there should be no errors
    x_tex = x_tex < 0 ? 0 : x_tex;
    y_tex = y_tex < 0 ? 0 : y_tex;
    x_tex = x_tex > texture.width()  ? texture.width()  - 1 : x_tex;
    y_tex = y_tex > texture.height() ? texture.height() - 1 : y_tex;

    return {x_tex, y_tex};
  }

  inline Point2 get_point_f_on_uv  (const unsigned x,
                                    const unsigned y) const {

    Matrix m ({{double(x) - t_origin.X, double(y) - t_origin.Y}});

    auto m2 = m * basis_changer;

    double x_tex = (uv_origin.X + m2[0][0]);
    double y_tex = (uv_origin.Y + m2[0][1]);

    return {x_tex, y_tex};
  }
};

#endif // TEXTUREPROJECTOR_H
