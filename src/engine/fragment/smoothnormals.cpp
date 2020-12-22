#include "smoothnormals.h"

// x cut point between point-v1 line and v2-v3 line
Vector2 cut_point(const Point2& p, const Point2& v1,
                  const Point2& v2, const Point2& v3) {

  // Director vector for point to v1 line
  Vector2 dir_1 = v1 - p;

  // Director vector for v2 to v3 line
  Vector2 dir_2 = v2 - v3;

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

void SmoothNormals::operator ()(unsigned pixel_index) {
  auto& triangle = get_triangle_at_pixel_index(pixel_index);
  Point2i p1 = pixel_index_to_screen_coordenates(pixel_index);
  Point2 p {double(p1.X), double(p1.Y)};

  // Cut point between vertex-point line and vertex-vertex line
  Vector2 ap_bc = cut_point(p, triangle.a, triangle.b, triangle.c);
  Vector2 bp_ac = cut_point(p, triangle.b, triangle.a, triangle.c);
  Vector2 cp_ab = cut_point(p, triangle.c, triangle.a, triangle.b);

  // Modules of the vectors of each vertex to the opposite cut point
  double max_distance_A = Point2::vector_module(triangle.a - ap_bc);
  double max_distance_B = Point2::vector_module(triangle.b - bp_ac);
  double max_distance_C = Point2::vector_module(triangle.c - cp_ab);

  // Vectors from the vertices to p
  Vector2 A = triangle.a - p;
  Vector2 B = triangle.b - p;
  Vector2 C = triangle.c - p;

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


/*
  double min_x = std::min({triangle.a.X, triangle.b.X, triangle.c.X});
  double max_x = std::max({triangle.a.X, triangle.b.X, triangle.c.X});
  double min_y = std::min({triangle.a.Y, triangle.b.Y, triangle.c.Y});
  double max_y = std::max({triangle.a.Y, triangle.b.Y, triangle.c.Y});

  double y_distance = max_y - min_y;
  double x_distance = max_x - min_x;

  Vector2 distances {x_distance, y_distance};





  // Influence of each vertex (proximity to p)
  double A_d = Point2::vector_module(A) ;
  double B_d = Point2::vector_module(B) ;
  double C_d = Point2::vector_module(C) ;

  Vector2 final_vec = (n_a / A_d) + (n_b / B_d) + (n_c / C_d);

  double x = final_vec.X;
  double y = final_vec.Y;
*/
  unsigned char r = 128 + std::round(127.0 * x);
  unsigned char g = 128 + std::round(127.0 * y);

  buffers.normal_buffer.set(pixel_index * 3 + 0, r);
  buffers.normal_buffer.set(pixel_index * 3 + 1, g);
  buffers.normal_buffer.set(pixel_index * 3 + 2, 255);
}
