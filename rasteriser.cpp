#include "rasteriser.h"

Rasteriser::Rasteriser(Canvas* cv, Camera* cm, World* wd) :
  canvas (cv),
  camera (cm),
  world (wd)
{}

#include <iostream>

void Rasteriser::rasterize() {

  Basis3 basis {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
  };

  // Transform camera to new basis
  Camera* aux_camera = camera->express_in_different_basis(basis);

  camera_plane = camera->get_plane();
  camera_bounds = aux_camera->get_bounds();
  camera_fuge = aux_camera->get_fuge();

  std::vector <Triangle2> projected_elements;

  std::vector <Face3> projected_faces;

  for (const auto& mesh : world->get_elements()) {
    // Change basis
    Mesh* aux_mesh = mesh->express_in_different_basis(basis);
    for (const auto& face : aux_mesh->faces) {

      // Calculate intersection points with the plane
      Point3 a = calculate_cut_point(face.a);
      Point3 b = calculate_cut_point(face.b);
      Point3 c = calculate_cut_point(face.c);

      projected_faces.push_back({a, b, c});
    }
  }

  // Adjust to camera basis
  Matrix3 M2 = SpatialOps::generate_basis_change_matrix (basis, camera->basis);
  for (auto& face : projected_faces) {
    Point3 a = SpatialOps::change_basis(M2, face.a);
    Point3 b = SpatialOps::change_basis(M2, face.b);
    Point3 c = SpatialOps::change_basis(M2, face.c);

    Point2 a2D = {a.x, a.y};
    Point2 b2D = {b.x, b.y};
    Point2 c2D = {c.x, c.y};

    bool should_be_rendered = false;

    if (is_point_between_camera_bounds(a2D) ||
        is_point_between_camera_bounds(b2D) ||
        is_point_between_camera_bounds(c2D)) {

      should_be_rendered = true;
    }

    // If should be rendered push a triangle
    if (should_be_rendered)
      projected_elements.push_back(Triangle2{a2D, b2D, c2D});
  }

  canvas->update_frame(projected_elements, camera_bounds);
}

bool Rasteriser::is_point_between_camera_bounds(Point2 p) {
  return p.x > camera_bounds.x     &&
         p.x < camera_bounds.width &&
         p.y > camera_bounds.y     &&
         p.y < camera_bounds.height;
}

Point3 Rasteriser::calculate_cut_point(Point3 p) {
  Point3 p1 = camera_fuge;
  Point3 p2 = p;


  // Vector director de la recta
  Vector3 v1 = {p2.x - p1.x,
                p2.y - p1.y,
                p2.z - p1.z };

  // Recta
  Line3 r = {p1, v1, 0};

  // Calcular el punto de corte recta - plano

  double A = camera_plane.x;
  double B = camera_plane.y;
  double C = camera_plane.z;
  double D = -80;

  double a = p1.x;
  double c = p1.y;
  double e = p1.z;

  double b = v1.x;
  double d = v1.y;
  double f = v1.z;

  double T1 = D - (A*a + B*c + C*e);
  double T2 = (A*b + B*d + C*f);

  double parameter = T1 / T2;

  Point3 cut_point = {
    p1.x + parameter * v1.x,
    p1.y + parameter * v1.y,
    p1.z + parameter * v1.z,
  };

  // Intersection in global coordiantes
  Point3 absolute_point = { r.point.x + r.vector.x * parameter,
                            r.point.y + r.vector.y * parameter,
                            r.point.z + r.vector.z * parameter,
                          };

  return absolute_point;
}
