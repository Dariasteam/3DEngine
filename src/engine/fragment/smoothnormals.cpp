#include "smoothnormals.h"

// x cut point between point-v1 line and v2-v3 line
Vector2 SmoothNormals::cut_point(const Point2& p, const Point3& v1,
                  const Point3& v2, const Point3& v3) const {

  // Director vector for point to v1 line
  Vector2 dir_1 = Point2(v1.x(), v1.y()) - p;

  // Director vector for v2 to v3 line
  const auto A = v2 - v3;
  Vector2 dir_2 = {A.x(), A.y()};

  dir_1 += {0.0000001, 0.000001};
  dir_2 += {0.0000001, 0.000001};

  double T1 = (v3.y() - p.y()) * (dir_1.x() * dir_2.x());
  double T2 = dir_2.x() * dir_1.y() * p.x();
  double T3 = dir_1.x() * dir_2.y() * v3.x();
  double T4 = dir_2.x() * dir_1.y() - dir_1.x() * dir_2.y();

  double X = (T1 + T2 - T3) / T4 ;

  double Y = ((dir_2.Y * (X - v3.X)) / dir_2.X) + v3.Y;

  return {X, Y};
}

/* 1. Create vectors from each vertex to the point (vec1)
 * 2. Create vectors from each vertex to the opposite edge passing through the point (vec2)
 *
 * 3. Assign a weight as normalice(vec1) / normalice(vec2) to each vertex
 *
 *
 * 4. Calculate the influece of each vertex normal with the weights and assign
 * to the current point
 */

void SmoothNormals::operator ()(unsigned pixel_index) {
  auto& triangle = get_triangle_at_pixel_index(pixel_index);
  Point2i p1 = pixel_index_to_screen_coordenates(pixel_index);
  Point2 p {double(p1.X), double(p1.Y)};

  // Cut point between vertex-point line and vertex-vertex line
  Vector2 ap_bc = cut_point(p, triangle.a, triangle.b, triangle.c);
  Vector2 bp_ac = cut_point(p, triangle.b, triangle.a, triangle.c);
  Vector2 cp_ab = cut_point(p, triangle.c, triangle.a, triangle.b);

  // Modules of the vectors of each vertex to the opposite cut point
  double max_distance_A = Point2::vector_module(Vector2{triangle.a.x(), triangle.a.y()} - ap_bc);
  double max_distance_B = Point2::vector_module(Vector2{triangle.b.x(), triangle.b.y()} - bp_ac);
  double max_distance_C = Point2::vector_module(Vector2{triangle.c.x(), triangle.c.y()} - cp_ab);

  // Vectors from the vertices to p
  Vector2 A = Vector2{triangle.a.x(), triangle.a.y()} - p;
  Vector2 B = Vector2{triangle.b.x(), triangle.b.y()} - p;
  Vector2 C = Vector2{triangle.c.x(), triangle.c.y()} - p;

  // Influence of each vertex (proximity of each vertex to p)
  double A_d = 1.0 - Point2::vector_module(A) / max_distance_A;
  double B_d = 1.0 - Point2::vector_module(B) / max_distance_B;
  double C_d = 1.0 - Point2::vector_module(C) / max_distance_C;

  // Normal vectors of the vertices
  Vector2 n_a = {triangle.normal_a.X, triangle.normal_a.Y};
  Vector2 n_b = {triangle.normal_b.X, triangle.normal_b.Y};
  Vector2 n_c = {triangle.normal_c.X, triangle.normal_c.Y};

  Vector2 n = {triangle.normal.x(), triangle.normal.y()};

  Vector2 final_vec = (n_a * A_d) + (n_b * B_d) + (n_c * C_d);


  double x = final_vec.X;
  double y = final_vec.Y;

  unsigned char r = 128 + std::round(127.0 * x);
  unsigned char g = 128 + std::round(127.0 * y);

  buffers.normal_buffer.set(pixel_index * 3 + 0, r);
  buffers.normal_buffer.set(pixel_index * 3 + 1, g);
  buffers.normal_buffer.set(pixel_index * 3 + 2, 255);
}
