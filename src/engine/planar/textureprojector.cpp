#include "textureprojector.h"

TextureProjector::TextureProjector() {}

void TextureProjector::generate_uv_projector(const Triangle& projected_triangle,
                                             const UV& uv) {
  t_origin  = projected_triangle.a;
  uv_origin = uv.p;

  // UV of the triangle as Texture basis (destination)
  Basis2 texture_basis {
    {(uv.u.X - uv.p.X), (uv.u.Y - uv.p.Y)},
    {(uv.v.X - uv.p.X), (uv.v.Y - uv.p.Y)}
  };

  // Projected triangle vertices as base of the screen (origin)
  Basis2 screen_basis ({
               {double(projected_triangle.b.X - projected_triangle.a.X),
                  double(projected_triangle.b.Y - projected_triangle.a.Y)},

               {double(projected_triangle.c.X - projected_triangle.a.X),
                  double(projected_triangle.c.Y - projected_triangle.a.Y)},
             });

  // Generate matrix to change between basis
  MatrixOps::generate_basis_change_matrix(texture_basis,
                                          screen_basis,
                                          basis_changer);  
}

Color888 TextureProjector::get_color_on_uv(const int x, const int y,
                                           const Texture<unsigned char, 3>& tex) const {

  const auto& point = get_point_on_uv(x, y, tex);
  return {
    tex.get(point.X, point.Y, 0),
    tex.get(point.X, point.Y, 1),
    tex.get(point.X, point.Y, 2),
  };
}

Point2i TextureProjector::get_point_on_uv(const int x, const int y, const Texture<unsigned char, 3>& texture) const {
  Matrix m ({{double(x - t_origin.X), double(y - t_origin.Y)}});

  auto m2 = m * basis_changer;  

  int x_tex = std::round((uv_origin.X + m2[0][0]) * texture.get_width());
  int y_tex = std::round((uv_origin.Y + m2[0][1]) * texture.get_height());

  // FIXME: This should not exist, there should be no errors
  if (x_tex < 0 || x_tex > texture.get_width() ||
      y_tex < 0 || y_tex > texture.get_height()) {    

    x_tex = std::min({x_tex, texture.get_width()});
    x_tex = std::max({x_tex, 0});

    y_tex = std::min({y_tex, texture.get_height()});
    y_tex = std::max({y_tex, 0});
  }

  return {x_tex, y_tex};
}
