#include "textureprojector.h"

TextureProjector::TextureProjector() :
  screen(1200, 600)
{}

/* Considering a triangle with the form
 *      v1
 *    /   \
 *   /     \
 *  v2 _____v3
 *
 *
 * Finally, we find the color of l3[y][x]
 *
 * */
void TextureProjector::fillBottomFlatTriangle(const Triangle2i& triangle,
                                              const Texture& tex) {
  auto v1 = triangle.a;
  auto v2 = triangle.b;
  auto v3 = triangle.c;

  if (v3.y() == v1.y()) return; // we already know v2 = v3, triangle would be a line

  if (v2.x() > v3.x()) // order from left to right
    std::swap(v2, v3);

  double invslope1 = double(v2.x()  - v1.x()) / (v2.y() - v1.y());
  double invslope2 = double(v3.x()  - v1.x()) / (v3.y() - v1.y());

  double curx1 = v1.x();
  double curx2 = curx1;

  int y1 = v1.y();
  int y2 = v2.y();

  for (int y = y1; y <= y2; y++) {
    int min_x = static_cast<int>(std::round(curx1));
    int max_x = static_cast<int>(std::round(curx2));

    for (int x = min_x; x <= max_x; x++) {
      /*
      if (triangle.z_value < z_buffer[y][x]) {
        (*screen_buffer)[y][x] = triangle.color;
                z_buffer[y][x] = triangle.z_value;
      }
      */
      paint_to_screen(x, y, tex);
    }
    curx1 += invslope1;
    curx2 += invslope2;
  }
}

/* Considering a triangle with the form
 *
 *  v1 _____v2
 *   \    /
 *    \  /
 *     v3
 *
 *
 * Finally, we find the color of l3[y][x]
 *
 * */
void TextureProjector::fillTopFlatTriangle(const Triangle2i& triangle,
                                           const Texture& tex) {

  auto v1 = triangle.a;
  auto v2 = triangle.b;
  auto v3 = triangle.c;

  if (v3.y() == v1.y()) return; // we already know v2 = v1, triangle would be a line

  if (v1.x() > v2.x()) // order from left to right
    std::swap(v1, v2);

  double invslope1 = double(v3.x() - v1.x()) / (v3.y() - v1.y());
  double invslope2 = double(v3.x() - v2.x()) / (v3.y() - v2.y());

  double curx1 = v3.x();
  double curx2 = curx1;

  int y3 = v3.y();
  int y1 = v1.y();

  for (int y = y3; y >= y1; y--) {
    int min_x = static_cast<int>(std::round(curx1));
    int max_x = static_cast<int>(std::round(curx2));

    for (int x = min_x; x <= max_x; x++) {
      /*
      if (triangle.z_value < z_buffer[y][x]) {
        (*screen_buffer)[y][x] = triangle.color;
                z_buffer[y][x] = triangle.z_value;
      }*/

      paint_to_screen(x, y, tex);
    }
    curx1 -= invslope1;
    curx2 -= invslope2;
  }
}

void TextureProjector::paint_to_screen(unsigned x,
                                       unsigned y,
                                       const Texture& tex) {
  Matrix m ({
              {double(x - t_origin.X),
               double(y - t_origin.Y)}
            });

  auto m2 = m * basis_changer;

  int x_tex = std::round(m2[0][0]);
  int y_tex = std::round(m2[0][1]);
/*
  if (x >= 0 && x <= screen.get_width() &&
      y >= 0 && y <= screen.get_height()) {
*/
    screen.set(x, y, 0, tex.get(x_tex, y_tex, 0));
    screen.set(x, y, 1, tex.get(x_tex, y_tex, 1));
    screen.set(x, y, 2, tex.get(x_tex, y_tex, 2));
/*
  } else {
    std::cout << "Error: " << x << " " << y << std::endl;
  }
  */
}

void TextureProjector::rasterize_triangle (Triangle2i& triangle, const Texture& tex) {

  t_origin = triangle.a;

  // Sort vertices by Y
  std::vector<Point2i> aux_vec = {triangle.a, triangle.b, triangle.c};
  std::sort (aux_vec.begin(), aux_vec.end(), [&](const Point2i& a, const Point2i& b) {
    return std::isless(a.y(), b.y());
  });

  triangle.a = aux_vec[0];
  triangle.b = aux_vec[1];
  triangle.c = aux_vec[2];

  const Point2i& v1 = aux_vec[0];
  const Point2i& v2 = aux_vec[1];
  const Point2i& v3 = aux_vec[2];

  // aux_triangle is ordered by y (v1 < v2 < v3)
  if (v2.y() == v3.y()) {
    fillBottomFlatTriangle(triangle, tex);
  } else if (v1.y() == v2.y()) {
    fillTopFlatTriangle(triangle, tex);
  } else {

    double a = v3.y() - v1.y();
    double b = v3.x() - v1.x();

    double ratio = b / a;
    double x = v1.x() + (ratio * (v2.y() - v1.y()));

    Point2i v4 (static_cast<int>(std::round(x)), v2.y());

    Triangle2i aux_t1 {triangle};
    Triangle2i aux_t2 {triangle};

    aux_t1.c = v4;

    aux_t2.a = v2;
    aux_t2.b = v4;

    fillBottomFlatTriangle (aux_t1, tex);
    fillTopFlatTriangle    (aux_t2, tex);
  }
}

void TextureProjector::project(const Texture& tex,
                               Triangle2i& projected_triangle,
                               const UV& uv) {

  // UV of the triangle as Texture basis (destination)
  Basis2 texture_basis {
    {(uv.u.X - uv.p.X) * tex.get_width(), (uv.u.Y - uv.p.Y) * tex.get_height()},
    {(uv.v.X - uv.p.X) * tex.get_width(), (uv.v.Y - uv.p.Y) * tex.get_height()}
  };

  // Screen basis in base of projected triangle points (origin)
  Basis2 screen_basis ({
               {double(projected_triangle.b.X - projected_triangle.a.X),
                  double(projected_triangle.b.Y - projected_triangle.a.Y)},

               {double(projected_triangle.c.X - projected_triangle.a.X),
                  double(projected_triangle.c.Y - projected_triangle.a.Y)},
             });

  // Generate matrix to change between basis  
  MatrixOps::generate_basis_change_matrix(texture_basis, screen_basis, basis_changer);

  std::cout << "Matrix\n" << basis_changer[0][0] << " "
                          << basis_changer[0][1] << "\n"
                          << basis_changer[1][0] << " "
                          << basis_changer[1][1] << "\n";
  // Populate screen
  rasterize_triangle (projected_triangle, tex);

  screen.write("screen.ppm");
}

void TextureProjector::generate_uv_projector(const Texture& tex,
                                             const Triangle2i& projected_triangle,
                                             const UV& uv) {
  t_origin = projected_triangle.a;
  texture = &tex;

  // UV of the triangle as Texture basis (destination)
  Basis2 texture_basis {
    {(uv.u.X - uv.p.X) * tex.get_width(), (uv.u.Y - uv.p.Y) * tex.get_height()},
    {(uv.v.X - uv.p.X) * tex.get_width(), (uv.v.Y - uv.p.Y) * tex.get_height()}
  };

  // Screen basis in base of projected triangle points (origin)
  Basis2 screen_basis ({
               {double(projected_triangle.b.X - projected_triangle.a.X),
                  double(projected_triangle.b.Y - projected_triangle.a.Y)},

               {double(projected_triangle.c.X - projected_triangle.a.X),
                  double(projected_triangle.c.Y - projected_triangle.a.Y)},
             });

  // Generate matrix to change between basis
  MatrixOps::generate_basis_change_matrix(texture_basis, screen_basis, basis_changer);
}
