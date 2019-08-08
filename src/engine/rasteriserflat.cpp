#include "rasteriserflat.h"

#include "rasteriserinterpolatedvertex.h"

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
void RasteriserFlat::fillBottomFlatTriangle(const Triangle2& triangle,
                            std::vector<std::vector<Color888>>* screen_buffer) {
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
      if (triangle.z_value < z_buffer[y][x]) {
        (*screen_buffer)[y][x] = triangle.color;
                z_buffer[y][x] = triangle.z_value;
      }
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
 * First we calculate the gradient ratio of the lines l1 = v3 - v1,
 * and l2 = v3 - v2
 *
 * Then in base of the Y of the point, we calculate the linear grading of
 * l3 = l1[y] - l2[y]
 *
 * Finally, we find the color of l3[y][x]
 *
 * */
void RasteriserFlat::fillTopFlatTriangle(const Triangle2& triangle,
                            std::vector<std::vector<Color888>>* screen_buffer) {
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
      if (triangle.z_value < z_buffer[y][x]) {
        (*screen_buffer)[y][x] = triangle.color;
                z_buffer[y][x] = triangle.z_value;
      }
    }
    curx1 -= invslope1;
    curx2 -= invslope2;
  }
}

inline bool is_equal (double a, double b) {
  return std::isless(std::abs(a - b), 0.00001);
}

void RasteriserFlat::rasterize_triangle (Triangle2& triangle,
                                std::vector<std::vector<Color888>>* screen_buffer) {

  // Sort vertices by Y
  std::vector<Vertex2> aux_vec = {triangle.a, triangle.b, triangle.c};
  std::sort (aux_vec.begin(), aux_vec.end(), [&](const Vertex2& a, const Vertex2& b) {
    return std::isless(a.y(), b.y());
  });

  triangle.a = aux_vec[0];
  triangle.b = aux_vec[1];
  triangle.c = aux_vec[2];

  const Vertex2& v1 = aux_vec[0];
  const Vertex2& v2 = aux_vec[1];
  const Vertex2& v3 = aux_vec[2];

  // aux_triangle is ordered by y (v1 < v2 < v3)
  if (v2.y() == v3.y()) {
    fillBottomFlatTriangle(triangle, screen_buffer);
  } else if (v1.y() == v2.y()) {
    fillTopFlatTriangle(triangle, screen_buffer);
  } else {

    double a = v3.y() - v1.y();
    double b = v3.x() - v1.x();

    double ratio = b / a;
    double x = v1.x() + (ratio * (v2.y() - v1.y()));

    Vertex2 v4 (static_cast<int>(std::round(x)), v2.y());

    Triangle2 aux_t1 {triangle};
    Triangle2 aux_t2 {triangle};

    aux_t1.c = v4;

    aux_t2.a = v2;
    aux_t2.b = v4;

    fillBottomFlatTriangle (aux_t1, screen_buffer);
    fillTopFlatTriangle    (aux_t2, screen_buffer);
  }
}
