#include "rasteriser.h"

Rasteriser::Rasteriser(Canvas* cv, Camera* cm, World* wd) :
  canvas (cv),
  camera (cm),
  world (wd)
{}

void Rasteriser::rasterize() {
  camera_fuge = camera->get_fuge();
  camera_plane = camera->get_plane();
  camera_bounds = camera->get_bounds();

  Basis3 basis {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
  };

  std::vector <Triangle2> projected_elements;
  for (const auto& mesh : world->get_elements()) {
    // Change basis
    Mesh* aux_mesh = mesh->express_in_different_basis(basis);
    for (const auto& face : aux_mesh->faces) {

      // Calculate intersection points with the plane

      Point2 a = calculate_cut_point(face.a);
      Point2 b = calculate_cut_point(face.b);
      Point2 c = calculate_cut_point(face.c);

      // Check if at least one is captured by camera

      bool should_be_rendered = false;

      if (is_point_between_camera_bounds(a) &&
          is_point_between_camera_bounds(b) &&
          is_point_between_camera_bounds(c)) {

        should_be_rendered = true;
      }

      // If should be rendered push a triangle
      if (should_be_rendered)
        projected_elements.push_back(Triangle2{a,b,c});
    }
  }
  canvas->update_frame(projected_elements, camera_bounds);
}

bool Rasteriser::is_point_between_camera_bounds(Point2 p) {
  return p.x > camera_bounds.x     &&
         p.x < camera_bounds.width &&
         p.y > camera_bounds.y     &&
         p.y < camera_bounds.height;
}

Point2 Rasteriser::calculate_cut_point(Point3 p) {
  Point3 p1 = camera_fuge;
  Point3 p2 = p;

  // Vector director de la recta
  Vector3 v1 = {p2.x - p1.x,
                p2.y - p1.y,
                p2.z - p1.z };

  // Recta
  Line3 r = {p1, v1, 0};

  // Punto de corte recta - plano
  double parameter = camera_plane.z / (r.point.z + r.vector.z);

  return { r.point.x + r.vector.x * parameter,
           r.point.y + r.vector.y * parameter,
          };
}
