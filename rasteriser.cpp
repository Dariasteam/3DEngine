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

  std::vector <Point2> projected_elements;
  for (const auto&  element : world->get_elements()) {
    Point2 aux = calculate_cut_point(element);
    if (is_point_between_camera_bounds(aux))
      projected_elements.push_back(aux);
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

  // Punto común
  double parameter = (r.point.z + r.vector.z) / camera_plane.z;

  return { r.point.x + r.vector.x * parameter,
           r.point.y + r.vector.y * parameter,
          };
}
