#include "rasteriser.h"

Rasteriser::Rasteriser(Canvas* cv, Camera* cm, World* wd) :
  canvas (cv),
  camera (cm),
  world (wd),
  projected_elements (N_THREADS)
{
  canvas->set_triangles_buffer(&elements_to_render);

  for (auto& element : projected_elements)
    element.resize(3000);

}

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
  unsigned segments = (size / N_THREADS);

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
  elements_to_render.clear();
  generate_mesh_list(world->get_elements());  
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

unsigned Rasteriser::calculate_mesh_projection(const Mesh* const mesh,
                                               const Matrix3& M2,
                                               std::vector<Triangle2>& triangles,
                                               unsigned index,
                                               Auxiliar& aux) {
  unsigned list_size = triangles.size();

  for (const auto& face : mesh->global_coordinates_faces) {
    // 1. Check if the face is loking torwards to the camera
    //double angle_torwards_camera = Vector3::angle_between(face.normal, camera_plane_vector);
    //if ((angle_torwards_camera < 90 || angle_torwards_camera > 270)) break;

    // 2. Calculate z value (distance to camera)
    Vector3::create_vector(face.a, camera_fuge, aux.v1);
    Vector3::create_vector(face.b, camera_fuge, aux.v2);
    Vector3::create_vector(face.c, camera_fuge, aux.v3);

    double z_min = std::min({aux.v1.z(), aux.v2.z(), aux.v3.z()});
    double z_max = std::max({aux.v1.z(), aux.v2.z(), aux.v3.z()});
    if (z_min > 10000) break;

    // 3. Calculate intersection points with the plane
    bool visible  = calculate_cut_point(face.a, aux.a)
                  | calculate_cut_point(face.b, aux.b)
                  | calculate_cut_point(face.c, aux.c);
    if (!visible) break;

    // 4. Transform to camera basis
    Point3Ops::change_basis(M2, aux.a, aux.a);
    Point3Ops::change_basis(M2, aux.b, aux.b);
    Point3Ops::change_basis(M2, aux.c, aux.c);

    //if (index < list_size) {
      triangles[index].a.set_values(aux.a.x(), aux.a.y());
      triangles[index].b.set_values(aux.b.x(), aux.b.y());
      triangles[index].c.set_values(aux.c.x(), aux.c.y());
      triangles[index].z_value = z_max;
      /*
    } else {
      triangles.push_back({{a.x(), a.y()},
                           {b.x(), b.y()},
                           {c.x(), c.y()},
                           z_max, {0,0,0}});
    }
    */
/*
    // 5. Generate projected 2D points
    Point2 a2D {a.x(), a.y()};
    Point2 b2D {b.x(), b.y()};
    Point2 c2D {c.x(), c.y()};
*/
    // 6. Check point in camera bounds
    bool point_in_camera = is_point_between_camera_bounds(triangles[index].a)
                         | is_point_between_camera_bounds(triangles[index].b)
                         | is_point_between_camera_bounds(triangles[index].c);
    if (!point_in_camera) {break;}

    // 7. Calculate light contribution
    const Color& color = calculate_lights(mesh->color, face);    

    // 8. Add triangle to the list          
    triangles[index].color = color;
    index++;
  }
  return index;
}

void Rasteriser::rasterise() {
  set_rasterization_data();

  Matrix3 M2;
  MatrixOps::generate_basis_change_matrix (world->basis, camera->basis, M2);

  unsigned segments = (meshes_vector.size() / N_THREADS);

  auto lambda = [&](unsigned init, unsigned end, unsigned vec_index) {
    Point3 a;
    Point3 b;
    Point3 c;    

    unsigned counter = 0;

    Auxiliar aux;

    for (unsigned j = init; j < end; j++)
      counter = calculate_mesh_projection(meshes_vector[j], M2,
                                          projected_elements[vec_index],
                                          counter, aux);

    while(!mtx.try_lock());    
    for (unsigned i = 0; i < counter; i++)
      elements_to_render.push_back(&projected_elements[vec_index][i]);
    mtx.unlock();
  };

  std::vector<std::future<void>> promises (N_THREADS);
  for (unsigned i = 0; i <N_THREADS  - 1; i++)
    promises[i] = std::async(lambda, i * segments, (i + 1) * segments, i);
  promises[N_THREADS - 1] = std::async(lambda, (N_THREADS - 1) * segments,
                                               meshes_vector.size(),
                                               N_THREADS - 1);
  for (auto& promise : promises)
    promise.get();

  // Order by distance to camera
  std::sort(elements_to_render.begin(), elements_to_render.end(),
    [](const Triangle2* a, const Triangle2* b) {
      return a->z_value > b->z_value;
  });

  canvas->update_frame(camera_bounds);
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

  double T1 {- D - (A*a + B*c + C*e)};
  double T2 {A*b + B*d + C*f};

  bool return_value = true;

  // Some vertex are behind camera
  if ((f < 0 && C > 0) || (f > 0 && C < 0)) {
      T1 = -T1;
      return_value = false;
  }

  double parameter = T1 / T2;

  // Intersection in global coordiantes
  point.set_x(-(a + b * parameter));
  point.set_y(-(c + d * parameter));
  point.set_z(  e + f * parameter);

  return return_value;
}
