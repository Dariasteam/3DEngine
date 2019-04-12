#include "rasteriser.h"

Rasteriser::Rasteriser(Canvas* cv, Camera* cm, World* wd) :
  canvas (cv),
  camera (cm),
  world (wd)
{}

void Rasteriser::generate_mesh_list(const std::vector<Mesh*> &list) {
  for (const auto& mesh : list) {
    meshes_list.push_back(mesh);
    generate_mesh_list(mesh->get_nested_meshes());
  }
}

void Rasteriser::rasterize() {
  // Transform camera to new basis
  camera->express_in_different_basis(world->basis);

  camera_plane_vector = camera->get_plane_vector();
  camera_bounds       = camera->get_bounds();
  camera_fuge         = camera->get_fuge();
  camera_plane_point  = camera->get_plane_point();

  std::vector <Triangle2>* projected_elements = new std::vector<Triangle2>;
  const Matrix3& M2 = MatrixOps::generate_basis_change_matrix (world->basis, camera->basis);

  // Change basis
  for (const auto& mesh : world->get_elements())
    mesh->express_in_parents_basis(world->basis);

  // Generate iterable list of meshes
  meshes_list.clear();
  generate_mesh_list(world->get_elements());

  Point3 a;
  Point3 b;
  Point3 c;

  for (const auto& mesh : meshes_list) {
    for (const auto& face : mesh->global_coordinates_faces) {
      bool visible = false;      

      // Calculate intersection points with the plane
      visible |= calculate_cut_point(face.a, a);
      visible |= calculate_cut_point(face.b, b);
      visible |= calculate_cut_point(face.c, c);

      if (visible) {
        // Use the camera basis
        Point3Ops::change_basis(M2, a, a);
        Point3Ops::change_basis(M2, b, b);
        Point3Ops::change_basis(M2, c, c);

        const Point2& a2D = {a.x(), a.y()};
        const Point2& b2D = {b.x(), b.y()};
        const Point2& c2D = {c.x(), c.y()};

        // Check if should be rendered
        bool should_be_rendered = is_point_between_camera_bounds(a2D) |
                                  is_point_between_camera_bounds(b2D) |
                                  is_point_between_camera_bounds(c2D);

        // If should be rendered push a triangle
        if (should_be_rendered) {
          // Calculate z value (distance to camera)
          const Vector3& v1 = Vector3::create_vector(face.a, camera_fuge);
          const Vector3& v2 = Vector3::create_vector(face.b, camera_fuge);
          const Vector3& v3 = Vector3::create_vector(face.c, camera_fuge);

          double z = std::max(std::max(v1.z(), v2.z()), v3.z());

          // Check if the face is loking torwards to the camera          
          double angle_torwards_camera = Vector3::angle_between(face.normal, v1);

          if (angle_torwards_camera >= 90 && angle_torwards_camera <= 270) {
            // The face should be rendered, check lightness

            const DirectionalLight& light = world->get_light();

            double angle_to_light = Vector3::angle_between(face.normal,
                                                           light.direction);

            Color color;

            double light_intensity = world->get_light().intensity;

            color.r = (double(180 - angle_to_light) / 180) * light.color.r;
            color.g = (double(180 - angle_to_light) / 180) * light.color.g;
            color.b = (double(180 - angle_to_light) / 180) * light.color.b;

            color.r = (double)(color.r) * (light_intensity / 255);
            color.g = (double)(color.g) * (light_intensity / 255);
            color.b = (double)(color.b) * (light_intensity / 255);

            color.r = (double)(color.r) * (mesh->color.r / 64);
            color.g = (double)(color.g) * (mesh->color.g / 64);
            color.b = (double)(color.b) * (mesh->color.b / 64);

            color.r = std::min (color.r, unsigned(255));
            color.g = std::min (color.g, unsigned(255));
            color.b = std::min (color.b, unsigned(255));

            projected_elements->push_back({a2D, b2D, c2D, z, color});
          }
        }
      }
    }    
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
  const Vector3& v = Vector3::create_vector(vertex, camera_fuge);

  // Calcular el punto de corte recta - plano

  const double& A = camera_plane_vector.x();
  const double& B = camera_plane_vector.y();
  const double& C = camera_plane_vector.z();

  const double& D = -(camera_plane_point.x() * A +
                      camera_plane_point.y() * B +
                      camera_plane_point.z() * C);

  const double& a = vertex.x();
  const double& c = vertex.y();
  const double& e = vertex.z();

  const double& b = v.x();
  const double& d = v.y();
  const double& f = v.z();

  double T1 = - D - (A*a + B*c + C*e);
  double T2 = (A*b + B*d + C*f);

  double parameter = T1 / T2;

  // Some vertex are behind camera
  if ((f < 0 && C > 0) || (f > 0 && C < 0)) {
      T1 = D - (A*a + B*c + C*e);
      T2 = (A*b + B*d + C*f);

      parameter = T1 / T2;

      point[0] = -(a + b * parameter);
      point[1] = -(c + d * parameter);
      point[2] =   e + f * parameter;

    return false;
  }

  // Intersection in global coordiantes
  point[0] = -(a + b * parameter);
  point[1] = -(c + d * parameter);
  point[2] =   e + f * parameter;

  return true;
}
