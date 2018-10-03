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

  camera_plane_vector = aux_camera->get_plane_vector();
  camera_bounds       = aux_camera->get_bounds();
  camera_fuge         = aux_camera->get_fuge();
  camera_plane_point  = aux_camera->get_plane_point();

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

  Basis2 basis2 {
    {1, 0},
    {0, 1},
  };

  // Adjust to camera basis

  Matrix3 M2 = SpatialOps::generate_basis_change_matrix (basis, camera->basis);
  Matrix2 M3 = PlanarOps::generate_basis_change_matrix (camera->get_projected_basis(), basis2);

  for (auto& face : projected_faces) {
    Point3 a = SpatialOps::change_basis(M2, face.a);
    Point3 b = SpatialOps::change_basis(M2, face.b);
    Point3 c = SpatialOps::change_basis(M2, face.c);

    Point2 a2D = {a.x, a.y};
    Point2 b2D = {b.x, b.y};
    Point2 c2D = {c.x, c.y};

    // Cambiar la base de los puntos del plano a la base de la pantalla

    a2D = PlanarOps::change_basis(M3, a2D);
    b2D = PlanarOps::change_basis(M3, b2D);
    c2D = PlanarOps::change_basis(M3, c2D);

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
  return p.x > camera_bounds.x       &&
         p.x < camera_bounds.width   &&
         p.y > camera_bounds.y       &&
         p.y < camera_bounds.height;
}

Point3 Rasteriser::calculate_cut_point(Point3 p) {

  // Vector director de la recta
  Vector3 v = {p.x - camera_fuge.x,
               p.y - camera_fuge.y,
               p.z - camera_fuge.z };

  // Calcular el punto de corte recta - plano

  double A = camera_plane_vector.x;
  double B = camera_plane_vector.y;
  double C = camera_plane_vector.z;

  double D = -(camera_plane_point.x * A +
               camera_plane_point.y * B +
               camera_plane_point.z * C);

  double a = p.x;
  double c = p.y;
  double e = p.z;

  double b = v.x;
  double d = v.y;
  double f = v.z;

  double T1 = D - (A*a + B*c + C*e);
  double T2 = (A*b + B*d + C*f);

  double parameter = T1 / T2;

  // Intersection in global coordiantes
  return { a + b * parameter,
           c + d * parameter,
           e + f * parameter,
         };
}
