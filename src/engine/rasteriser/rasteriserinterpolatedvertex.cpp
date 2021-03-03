#include "rasteriserinterpolatedvertex.h"

inline Color get_gradient (const Color& color1,
                                    const Color& color2,
                                    double p1,
                                    double p2) {

  double r_ratio = double(color2.x() - color1.x()) / double(p2 - p1);
  double g_ratio = double(color2.y() - color1.y()) / double(p2 - p1);
  double b_ratio = double(color2.z() - color1.z()) / double(p2 - p1);

  return Color (r_ratio, g_ratio, b_ratio);
}

inline Color get_color_in_gradient (const Color& color1,
                                    const Color& gradient_ratio,
                                    double p1,
                                    double pos) {

  double new_r = color1.x() + (gradient_ratio.x() * (pos - p1));
  double new_g = color1.y() + (gradient_ratio.y() * (pos - p1));
  double new_b = color1.z() + (gradient_ratio.z() * (pos - p1));

  return Color (new_r, new_g, new_b);
}

inline void RasteriserInterpolatedVertex::paintLine (const Triangle2i& triangle,
                                                     const int min_x,
                                                     const int max_x,
                                                     const int y,
                                                     const Color& gradient,
                                                     const Color& color) {

  for (int x = min_x; x <= max_x; x++) {
    if (triangle.z_value < z_buff[toDepthIndex(x, y)]) {
      Color aux = get_color_in_gradient(color, gradient, min_x, x);
      clamp_color(aux);      

      writeColorToCurrentBuffer(x, y, aux);
      writeDepthBuffer(x, y, triangle.z_value);
    }
  }
}

/* Considering a triangle with the form
 *      v1
 *    /   \
 *   /     \
 *  v2 _____v3
 *
 * First we calculate the gradient ratio of the lines l1 = v1 - v2,
 * and l2 = v1 - v3
 *
 * Then in base of the Y of the point, we calculate the linear grading of
 * l3 = l1[y] - l2[y]
 *
 * Finally, we find the color of l3[y][x]
 *
 * */
void RasteriserInterpolatedVertex::fillBottomFlatTriangle(const Triangle& triangle,
                                                          unsigned t_index) const {
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

  // grading between v1 and v2
  Color gradient_1 = get_gradient(v1.color,
                                  v2.color,
                                  v1.y(),
                                  v2.y());

  // grading between v1 and v3
  Color gradient_2 = get_gradient(v1.color,
                                  v3.color,
                                  v1.y(),
                                  v3.y());

  for (int y = y1; y <= y2; y++) {
    int min_x = static_cast<int>(std::round(curx1));
    int max_x = static_cast<int>(std::round(curx2));

    Color color1 = get_color_in_gradient(v2.color, gradient_1, v2.y(), y);
    Color color2 = get_color_in_gradient(v3.color, gradient_2, v3.y(), y);

    Color gradient_3 = get_gradient(color1,
                                    color2,
                                    min_x,
                                    max_x);

    paintLine(triangle, min_x, max_x, y, gradient_3, color1);

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
 * First we calculate the gradient ratio of the lines l1 = v3 - v1,
 * and l2 = v3 - v2
 *
 * Then in base of the Y of the point, we calculate the linear grading of
 * l3 = l1[y] - l2[y]
 *
 * Finally, we find the color of l3[y][x]
 *
 * */
void RasteriserInterpolatedVertex::fillTopFlatTriangle(const Triangle& triangle,
                                                       unsigned t_index) const {
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

  // grading between v3 and v1
  Color gradient_1 = get_gradient(v1.color,
                                  v3.color,
                                  v1.y(),
                                  v3.y());

  // grading between v3 and v1
  Color gradient_2 = get_gradient(v2.color,
                                  v3.color,
                                  v2.y(),
                                  v3.y());

  for (int y = y3; y >= y1; y--) {
    int min_x = static_cast<int>(std::round(curx1));
    int max_x = static_cast<int>(std::round(curx2));

    Color color1 = get_color_in_gradient(v1.color, gradient_1, v1.y(), y);
    Color color2 = get_color_in_gradient(v2.color, gradient_2, v2.y(), y);

    Color gradient_3 = get_gradient(color1,
                                    color2,
                                    min_x,
                                    max_x);


    paintLine(triangle, min_x, max_x, y, gradient_3, color1);

    curx1 -= invslope1;
    curx2 -= invslope2;
  }
}

inline bool is_equal (double a, double b) {
  return std::isless(std::abs(a - b), 0.00001);
}

void RasteriserInterpolatedVertex::rasterize_triangle (Triangle& triangle,
                                                       unsigned t_index) const {
  triangle_to_surface_space(triangle);

  // Sort vertices by Y
  std::vector<Point2> aux_vec = {triangle.a, triangle.b, triangle.c};
  std::sort (aux_vec.begin(), aux_vec.end(), [&](const Point2& a, const Point2& b) {
    return std::isless(a.y(), b.y());
  });

  Triangle tmp_triangle {triangle};
  tmp_triangle.a = aux_vec[0];
  tmp_triangle.b = aux_vec[1];
  tmp_triangle.c = aux_vec[2];

  const Point2& v1 = aux_vec[0];
  const Point2& v2 = aux_vec[1];
  const Point2& v3 = aux_vec[2];

  // aux_triangle is ordered by y (v1 < v2 < v3)
  if (v2.y() == v3.y()) {
    fillBottomFlatTriangle(triangle);
  } else if (v1.y() == v2.y()) {
    fillTopFlatTriangle(triangle);
  } else {

    double a = v3.y() - v1.y();
    double b = v3.x() - v1.x();

    double ratio = b / a;
    double x = v1.x() + (ratio * (v2.y() - v1.y()));

    Point2i v4 (static_cast<int>(std::round(x)), v2.y());
    Color gradient = get_gradient(v1.color,
                                  v3.color,
                                  v1.y(),
                                  v3.y());

    Color aux = get_color_in_gradient(v1.color, gradient, v1.y(), v4.y());
    clamp_color(aux);

    v4.color = aux;

    Triangle2i aux_t1 {triangle};
    Triangle2i aux_t2 {triangle};

    aux_t1.c = v4;

    aux_t2.a = v2;
    aux_t2.b = v4;

    fillBottomFlatTriangle (aux_t1);
    fillTopFlatTriangle    (aux_t2);
  }
}
