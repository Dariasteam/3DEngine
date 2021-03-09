#include "rasteriserflat.h"

/* Considering a triangle with the form
 *      u1
 *    /   \
 *   /     \
 *  u2 _____u3
 *
 * First we calculate the gradient ratio of the lines l1 = u1 - u2,
 * and l2 = u1 - u3
 *
 * Then in base of the Y of the point, we calculate the linear grading of
 * l3 = l1[y] - l2[y]
 *
 * Finally, we find the color of l3[y][x]
 *
 * */
void RasteriserFlat::fillBottomFlatTriangle(const Triangle& triangle,
                                            unsigned t_index) const {
  auto u1 = triangle.a;
  auto u2 = triangle.b;
  auto u3 = triangle.c;    

  if (u3.y() == u1.y()) return; // we already know u2 = u3, triangle would be a line

  if (u2.x() > u3.x()) // order from left to right
    std::swap(u2, u3);

  double invslope1 = double(u2.x()  - u1.x()) / (u2.y() - u1.y());
  double invslope2 = double(u3.x()  - u1.x()) / (u3.y() - u1.y());

  double curx1 = u1.x();
  double curx2 = curx1;

  const unsigned y1 = std::round(u1.y());
  const unsigned y2 = std::round(u2.y());

  for (unsigned y = y1; y <= y2; y++) {
    const unsigned min_x = std::round(curx1);
    const unsigned max_x = std::round(curx2);

    const auto v13 = u1 - u3;
    const auto v12 = u1 - u2;

    const double z_left  = (((double(y) - u1.y()) * v12.Z) / v12.y()) + u1.z();
    const double z_right = (((double(y) - u1.y()) * v13.Z) / v13.y()) + u1.z();

    const auto uL = Vector3{double(min_x), double(y), z_left};
    const auto uR = Vector3{double(max_x + 0.000001), double(y), z_right};

    const auto vLR = uL - uR;

    for (unsigned x = min_x; x <= max_x; x++) {      
     const double z = (((double(x) - uL.x()) * vLR.Z) / vLR.x()) + uL.z();
     update_buffers(x, y, z, t_index);
    }

    curx1 += invslope1;
    curx2 += invslope2;
  }
}

/* Considering a triangle with the form
 *
 *  u1 _____u2
 *   \    /
 *    \  /
 *     u3
 *
 * First we calculate the gradient ratio of the lines l1 = u3 - u1,
 * and l2 = u3 - u2
 *
 * Then in base of the Y of the point, we calculate the linear grading of
 * l3 = l1[y] - l2[y]
 *
 * Finally, we find the color of l3[y][x]
 *
 * */
void RasteriserFlat::fillTopFlatTriangle(const Triangle& triangle,
                                         unsigned t_index) const {
  auto u1 = triangle.a;
  auto u2 = triangle.b;
  auto u3 = triangle.c;

  if (u3.y() == u1.y()) return; // we already know u2 = u1, triangle would be a line

  if (u1.x() > u2.x()) // order from left to right
    std::swap(u1, u2);

  double invslope1 = double(u3.x() - u1.x()) / (u3.y() - u1.y());
  double invslope2 = double(u3.x() - u2.x()) / (u3.y() - u2.y());

  double curx1 = u3.x();
  double curx2 = curx1;

  const unsigned y3 = std::round(u3.y());
  const unsigned y1 = std::round(u1.y());

  for (unsigned y = y3; y >= y1; y--) {
    const unsigned min_x = std::round(curx1);
    const unsigned max_x = std::round(curx2);

    const auto v31 = u3 - u1;
    const auto v32 = u3 - u2;

    const double z_left  = (((double(y) - u3.y()) * v31.Z) / v31.y()) + u3.z();
    const double z_right = (((double(y) - u3.y()) * v32.Z) / v32.y()) + u3.z();

    // FIXME: +0.0000001 for the pure vertices case where xmin = xmax = 0
    const auto uL = Vector3{double(min_x), double(y), z_left};
    const auto uR = Vector3{double(max_x) + 0.000001, double(y), z_right};

    const auto vLR = uL - uR;

    for (unsigned x = min_x; x <= max_x; x++) {
      const double z = (((double(x) - uL.x()) * vLR.Z) / vLR.x()) + uL.z();
      update_buffers(x, y, z, t_index);
    }

    curx1 -= invslope1;
    curx2 -= invslope2;
  }
}

void RasteriserFlat::rasterize_triangle (Triangle& triangle, unsigned t_index) const {  
  triangle_to_surface_space(triangle);

  // Sort vertices by Y
  std::vector<Point3> aux_vec = {triangle.a,
                                 triangle.b,
                                 triangle.c};

  std::sort (aux_vec.begin(), aux_vec.end(), [&](const Point3& a, const Point3& b) {
    return std::isless(a.y(), b.y());
  });

  Triangle tmp_triangle {triangle};

  tmp_triangle.a = aux_vec[0];
  tmp_triangle.b = aux_vec[1];
  tmp_triangle.c = aux_vec[2];  

  Point3& u1 = aux_vec[0];
  Point3& u2 = aux_vec[1];
  Point3& u3 = aux_vec[2];

  // aux_triangle is ordered by y (u1 < u2 < u3)
  if (u2.y() == u3.y()) {
    fillBottomFlatTriangle(tmp_triangle, t_index);
  } else if (u1.y() == u2.y()) {
    fillTopFlatTriangle(tmp_triangle, t_index);
  } else {
    double a = u3.y() - u1.y();
    double b = u3.x() - u1.x();

    double ratio = b / a;
    double x = u1.x() + (ratio * (u2.y() - u1.y()));

    double y = u2.y();
    const auto v31 = u3 - u1;
    double z = (((y - u1.y()) * v31.z()) / v31.y()) + u1.z();

    const Point3 u4 (x, y, z);

    Triangle aux_t_bttm {tmp_triangle};
    Triangle aux_t_top {tmp_triangle};

    double aux_z;

    aux_t_bttm.c = u4;

    aux_t_top.a = u2;
    aux_t_top.b = u4;

    fillBottomFlatTriangle (aux_t_bttm, t_index);
    fillTopFlatTriangle    (aux_t_top, t_index);
  }
}
