#include "rasteriser.h"

Rasteriser::Rasteriser(Canvas* cv, Camera* cm, World* wd) :
  canvas (cv),
  camera (cm),
  world (wd)
{}

void Rasteriser::generate_mesh_list(const std::vector<Mesh*> &meshes) {
  for (const auto& mesh : meshes) {
    meshes_vector.push_back(mesh);
    generate_mesh_list(mesh->get_nested_meshes());
  }
}

void Rasteriser::set_rasterization_data() {
  // Transform camera to new basis
  camera->express_in_different_basis(world->basis);

  camera_plane_vector = camera->get_plane_vector();  
  camera_bounds       = camera->get_bounds();
  camera_fuge         = camera->get_fuge();
  camera_plane_point  = camera->get_plane_point();  

  // Change basis
  const auto& meshes = world->get_elements();
  unsigned size = meshes.size();  
  unsigned segments = std::floor(float(size) / N_THREADS);

  auto lambda = [&](unsigned init, unsigned end) {
    for (unsigned j = init; j < end; j++)
      meshes[j]->express_in_parents_basis(world->basis);
  };

  std::vector<std::future<void>> promises (N_THREADS);
  for (unsigned i = 0; i < N_THREADS - 1; i++)
    promises[i] = std::async(lambda, i * segments, (i + 1) * segments);
  promises[N_THREADS - 1] = std::async(lambda, (N_THREADS - 1) * segments, size);

  for (auto& promise : promises)
    promise.get();

  // Generate iterable list of meshes
  meshes_vector.clear();
  generate_mesh_list(world->get_elements());
  projected_elements = new std::list<Triangle2>;
}

Color Rasteriser::calculate_lights (const Color& m_color, const Face3& face) const {
  const DirectionalLight& light = world->get_light();

  double angle_to_light = Vector3::angle_between(face.normal,
                                                 light.direction);
  if (angle_to_light > 180) angle_to_light -= 180;

  Color color;
  double light_intensity = world->get_light().intensity;

  color.r = (double(180 - angle_to_light) / 180) * light.color.r;
  color.g = (double(180 - angle_to_light) / 180) * light.color.g;
  color.b = (double(180 - angle_to_light) / 180) * light.color.b;

  color.r = (double)(color.r) * (light_intensity / 255);
  color.g = (double)(color.g) * (light_intensity / 255);
  color.b = (double)(color.b) * (light_intensity / 255);

  color.r = (double)(color.r) * (m_color.r / 64);
  color.g = (double)(color.g) * (m_color.g / 64);
  color.b = (double)(color.b) * (m_color.b / 64);

  color.r = std::min (color.r, unsigned(255));
  color.g = std::min (color.g, unsigned(255));
  color.b = std::min (color.b, unsigned(255));
  return color;
}

#include <iostream>

void Rasteriser::calculate_mesh_projection(const Mesh* const mesh,
                                           const Matrix3& M2,
                                           std::list<Triangle2>& triangles,
                                           Point3& a,
                                           Point3& b,
                                           Point3& c) const {

  for (const auto& face : mesh->global_coordinates_faces) {
    // 1. Check if the face is loking torwards to the camera
    //double angle_torwards_camera = Vector3::angle_between(face.normal, camera_plane_vector);
    //if ((angle_torwards_camera < 90 || angle_torwards_camera > 270)) break;

    // 2. Calculate z value (distance to camera)
    const Vector3& v1 = Vector3::create_vector(face.a, camera_fuge);
    const Vector3& v2 = Vector3::create_vector(face.b, camera_fuge);
    const Vector3& v3 = Vector3::create_vector(face.c, camera_fuge);

    double z_min = std::min({v1.z(), v2.z(), v3.z()});
    double z_max = std::max({v1.z(), v2.z(), v3.z()});
    if (z_min > 5000) break;

    // 3. Calculate intersection points with the plane
    bool visible  = calculate_cut_point(face.a, a)
                  | calculate_cut_point(face.b, b)
                  | calculate_cut_point(face.c, c);
    if (!visible) break;

    // 4. Transform to camera basis
    Point3Ops::change_basis(M2, a, a);
    Point3Ops::change_basis(M2, b, b);
    Point3Ops::change_basis(M2, c, c);

    // 5. Generate projected 2D points
    Point2 a2D {a.x(), a.y()};
    Point2 b2D {b.x(), b.y()};
    Point2 c2D {c.x(), c.y()};

    // 6. Check point in camera bounds
    bool point_in_camera = is_point_between_camera_bounds(a2D)
                         | is_point_between_camera_bounds(b2D)
                         | is_point_between_camera_bounds(c2D);
    if (!point_in_camera) break;

    // 7. Calculate light contribution
    const Color& color = calculate_lights(mesh->color, face);

    // 8. Add triangle to the list
    triangles.push_back({a2D, b2D, c2D, z_max, color});
  }
}

void Rasteriser::rasterize() {
  set_rasterization_data();

  const Matrix3& M2 = MatrixOps::generate_basis_change_matrix (world->basis, camera->basis);

  unsigned segments = std::floor(float(meshes_vector.size()) / N_THREADS);

  auto lambda = [&](unsigned init, unsigned end) -> void{
    Point3 a;
    Point3 b;
    Point3 c;

    std::list<Triangle2> triangles;

    for (unsigned j = init; j < end; j++)
      calculate_mesh_projection(meshes_vector[j], M2, triangles, a, b, c);

    while(!mtx.try_lock());
    projected_elements->splice (projected_elements->end(), triangles);
    mtx.unlock();
  };

  std::vector<std::future<void>> promises (N_THREADS);
  for (unsigned i = 0; i <N_THREADS  - 1; i++)
    promises[i] = std::async(lambda, i * segments, (i + 1) * segments);
  promises[N_THREADS - 1] = std::async(lambda, (N_THREADS - 1) * segments, meshes_vector.size());

  for (auto& promise : promises)
    promise.get();

  canvas->update_frame(projected_elements, camera_bounds);
}

bool Rasteriser::is_point_between_camera_bounds(const Point2& p) const {
  return p.x() > camera_bounds.x       &&
         p.x() < camera_bounds.width   &&
         p.y() > camera_bounds.y       &&
         p.y() < camera_bounds.height;
}

// Calculate the intersection point between teh camera plane and the vertex
bool Rasteriser::calculate_cut_point(const Point3& vertex,
                                           Point3& point) const {

  // Dir vector of the line
  const Vector3& v = Vector3::create_vector(vertex, camera_fuge);

  // Calc cut point line - plane

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
