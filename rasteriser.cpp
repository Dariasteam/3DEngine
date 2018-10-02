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

  Camera* aux_camera = camera->express_in_different_basis(basis);

  camera_plane = camera->get_plane();
  camera_bounds = aux_camera->get_bounds();
  camera_fuge = aux_camera->get_fuge();

  std::cout << camera_plane.x << " "
            << camera_plane.y << " "
            << camera_plane.z << " "
            << std::endl;

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

      if (is_point_between_camera_bounds(a) ||
          is_point_between_camera_bounds(b) ||
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

  // Calcular el punto de corte recta - plano

  double A = camera_plane.x;
  double B = camera_plane.y;
  double C = camera_plane.z;
  double D = -10;

  double a = p1.x;
  double c = p1.y;
  double e = p1.z;

  double b = v1.x;
  double d = v1.y;
  double f = v1.z;

  double T1 = D - (A*a + B*c + C*e);
  double T2 = (A*b + B*d + C*f);

  double parameter = T1 / T2;

  return { r.point.x + r.vector.x * parameter,
           r.point.y + r.vector.y * parameter,
         };
}
