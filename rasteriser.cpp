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

  delete aux_camera;

  std::vector <Triangle2> projected_elements;
  std::vector <Face3> projected_faces;

  Matrix3 M2 = MatrixOps::generate_basis_change_matrix (world->basis3, camera->basis);

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

      if (visible) {
        // Use the camera basis
        a = MatrixOps::change_basis(M2, a);
        b = MatrixOps::change_basis(M2, b);
        c = MatrixOps::change_basis(M2, c);

        Point2 a2D = {a.x(), a.y()};
        Point2 b2D = {b.x(), b.y()};
        Point2 c2D = {c.x(), c.y()};

        // Check if should be rendered
        bool should_be_rendered = is_point_between_camera_bounds(a2D) |
                                  is_point_between_camera_bounds(b2D) |
                                  is_point_between_camera_bounds(c2D);

        // If should be rendered push a triangle
        if (should_be_rendered) {
          // Calculate z value (distance to camera)
          Vector3 v1 = Vector3::create_vector(face.a, camera_fuge);
          Vector3 v2 = Vector3::create_vector(face.b, camera_fuge);
          Vector3 v3 = Vector3::create_vector(face.c, camera_fuge);

          double z = (v1.z() + v2.z() + v3.z()) / 3;

          projected_elements.push_back({a2D, b2D, c2D, z});
        }
      }
    }
    delete aux_mesh;
  }  

  canvas->update_frame(projected_elements, camera_bounds);
}

inline bool Rasteriser::is_point_between_camera_bounds(const Point2& p) {
  return p.x() > camera_bounds.x       &&
         p.x() < camera_bounds.width   &&
         p.y() > camera_bounds.y       &&
         p.y() < camera_bounds.height;
}

// Calculate the intersection point between teh camera plane and the vertex
bool Rasteriser::calculate_cut_point(const Point3& vertex,
                                           Point3& point) {

  // Vector director de la recta
  Vector3 v = Vector3::create_vector(vertex, camera_fuge);

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
