#include "textureprojector.h"

TextureProjector::TextureProjector() {}

void TextureProjector::generate_uv_projector(const Triangle& projected_triangle,
                                             const UV& uv) {
  auto tmp = Vector2(projected_triangle.a.X, projected_triangle.a.Y);
  t_origin  = P2i{
    static_cast<int>(std::round(tmp.X)),
    static_cast<int>(std::round(tmp.Y))
  };

  uv_origin = uv.p;

  // UV of the triangle as Texture basis (destination)
  Basis2 texture_basis {
    {(uv.u.X - uv.p.X), (uv.u.Y - uv.p.Y)},
    {(uv.v.X - uv.p.X), (uv.v.Y - uv.p.Y)}
  };

  // Projected triangle vertices as base of the screen (origin)
  Basis2 screen_basis ({
               {projected_triangle.b.X - projected_triangle.a.X,
                projected_triangle.b.Y - projected_triangle.a.Y},

               {projected_triangle.c.X - projected_triangle.a.X,
                projected_triangle.c.Y - projected_triangle.a.Y},
             });

  // Generate matrix to change between basis
  MatrixOps::generate_basis_change_matrix(texture_basis,
                                          screen_basis,
                                          basis_changer);
}

Color888 TextureProjector::get_color_on_uv(const unsigned x,
                                           const unsigned y,
                                           const Texture<unsigned char, 3>& tex)
                                           const {

  const auto& point = get_point_on_uv(x, y, tex);
  return {
    tex.get(point.X, point.Y, 0),
    tex.get(point.X, point.Y, 1),
    tex.get(point.X, point.Y, 2),
  };
}
