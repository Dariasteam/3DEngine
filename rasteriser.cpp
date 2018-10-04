#include "rasteriser.h"

Rasteriser::Rasteriser(Canvas* cv, Camera* cm, World* wd) :
  canvas (cv),
  camera (cm),
  world (wd)
{}

void Rasteriser::rasterize() {
  // Transform camera to new basis
  Camera* aux_camera = camera->express_in_different_basis(world->basis3);

  camera_plane_vector = aux_camera->get_plane_vector();
  camera_bounds       = aux_camera->get_bounds();
  camera_fuge         = aux_camera->get_fuge();
  camera_plane_point  = aux_camera->get_plane_point();

  std::vector <Triangle2> projected_elements;
  std::vector <Face3> projected_faces;

  for (const auto& mesh : world->get_elements()) {
    // Change basis
    Mesh* aux_mesh = mesh->express_in_different_basis(world->basis3);
    for (const auto& face : aux_mesh->faces) {
      bool visible = false;

      Point3 a;
      Point3 b;
      Point3 c;

      // Calculate intersection points with the plane
      visible |= calculate_cut_point(face.a, a);
      visible |= calculate_cut_point(face.b, b);
      visible |= calculate_cut_point(face.c, c);

      if (visible)
        projected_faces.push_back(Face3({a, b, c}));
    }
  }  

  Matrix3 M2 = SpatialOps::generate_basis_change_matrix (world->basis3, camera->basis);
  Matrix2 M3 = PlanarOps::generate_basis_change_matrix (camera->get_projected_basis(), world->basis2);

  // Adjust to camera basis  
  for (auto& face : projected_faces) {
    Point3 a = SpatialOps::change_basis(M2, face.a);
    Point3 b = SpatialOps::change_basis(M2, face.b);
    Point3 c = SpatialOps::change_basis(M2, face.c);

    // Cambiar la base de los puntos del plano a la base de la pantalla
    Point2 a2D = PlanarOps::change_basis(M3, {a.x(), a.y()});
    Point2 b2D = PlanarOps::change_basis(M3, {b.x(), b.y()});
    Point2 c2D = PlanarOps::change_basis(M3, {c.x(), c.y()});

    // Comprobar si debe ser pintado
    bool should_be_rendered = is_point_between_camera_bounds(a2D) |
                              is_point_between_camera_bounds(b2D) |
                              is_point_between_camera_bounds(c2D);

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

// Calculate the intersection point between teh camera plane and the vertex
bool Rasteriser::calculate_cut_point(Point3 vertex, Point3& point) {

  // Vector director de la recta
  Vector3 v = {vertex.x() - camera_fuge.x(),
               vertex.y() - camera_fuge.y(),
               vertex.z() - camera_fuge.z() };

  // Calcular el punto de corte recta - plano

  double A = camera_plane_vector.x();
  double B = camera_plane_vector.y();
  double C = camera_plane_vector.z();

  double D = -(camera_plane_point.x() * A +
               camera_plane_point.y() * B +
               camera_plane_point.z() * C);

  double a = vertex.x();
  double c = vertex.y();
  double e = vertex.z();

  double b = v.x();
  double d = v.y();
  double f = v.z();

  double T1 = D - (A*a + B*c + C*e);
  double T2 = (A*b + B*d + C*f);

  double parameter = T1 / T2;  

  // Intersection in global coordiantes
  point = { a + b * parameter,
            c + d * parameter,
            e + f * parameter,  // actually we dont need Z because 2D projection
          };

  if (f < 0 && C > 0) {
    point = {-point.x(), -point.y(), point.z()};
    return false;
  } else {
    return true;
  }
}
