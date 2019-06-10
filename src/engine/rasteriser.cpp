#include "rasteriser.h"

Rasteriser::Rasteriser(Canvas* cv, Camera* cm, World* wd) :
  world (wd),
  camera (cm),
  canvas (cv),
  projected_elements (N_THREADS)
{
  canvas->set_triangles_buffer(&elements_to_render);

  //elements_to_render.reserve(1000000);

  for (auto& element : projected_elements)
    element.reserve(300000);
}

void Rasteriser::generate_mesh_list(const std::vector<Mesh*> &meshes) {
  for (const auto& mesh : meshes) {
    meshes_vector.push_back(mesh);
    generate_mesh_list(mesh->get_nested_meshes());
  }
}

void Rasteriser::set_rasterization_data() {
  const auto& meshes = world->get_elements();
  unsigned size = meshes.size();  
  unsigned segments = (size / N_THREADS);

  Vector3 camera_t;
  if (!camera->basis_changed)
    camera_t = camera->position;
  else
    camera_t = camera->translation;

  // Change basis to camera
  auto lambda = [&](unsigned init, unsigned end) {
    for (unsigned j = init; j < end; j++)
      meshes[j]->express_in_parents_basis(camera->basis,
                                          camera_t,
                                          camera->basis_changed,
                                          camera->position_changed);
  }; 

  std::vector<std::future<void>> promises (N_THREADS);
  for (unsigned i = 0; i < N_THREADS - 1; i++)
    promises[i] = std::async(lambda, i * segments, (i + 1) * segments);
  promises[N_THREADS - 1] = std::async(lambda,
                                       (N_THREADS - 1) * segments,size);

  for (auto& promise : promises)
    promise.get();

  camera->basis_changed = false;
  camera->position_changed = false;

  // Generate iterable list of meshes
  meshes_vector.clear();

  elements_to_render.clear();
  generate_mesh_list(world->get_elements());  
}

Color Rasteriser::calculate_lights (const Color& m_color,
                                    const Face3& face) const {

  const DirectionalLight& light = world->get_light();

  // Using dot product as angle
  double angle_to_light = 1 + (face.normal * light.direction);

  Color color = light.color;
  color *= angle_to_light * world->get_light().intensity;;

  color.set_x(std::min (fabs(color.x() * m_color.x() / 255), 254.0));
  color.set_y(std::min (fabs(color.y() * m_color.y() / 255), 254.0));
  color.set_z(std::min (fabs(color.z() * m_color.z() / 255), 254.0));

  return color;
}

bool Rasteriser::calculate_mesh_projection(const Face3& face,                                           
                                           std::vector<Triangle2>& triangles,
                                           unsigned index,                                           
                                           const Color& color) {  

  // 1. Check face is not behind camera. Since we are using camera basis
  // fugue is always at 0,0,0. Also each point coordinate is also
  // it's vector
  if (face.a.z() < 0.0 && face.b.z() < 0.0 && face.c.z() < 0.0) return false;

  // 2. Calculate distance to camera
  double mod_v1 = Vector3::vector_module(face.a);
  double mod_v2 = Vector3::vector_module(face.b);
  double mod_v3 = Vector3::vector_module(face.c);

  double z_min = std::min({mod_v1, mod_v2, mod_v3});
  double z_max = std::max({mod_v1, mod_v2, mod_v3});
  if (z_min > 10000) return false;

  Point3 pa, pb, pc;

  // 3. Calculate intersection points with the plane
  bool visible  = calculate_cut_point(face.a, face.a, pa)
                | calculate_cut_point(face.b, face.b, pb)
                | calculate_cut_point(face.c, face.c, pc);
  if (!visible) return false;

  // 4. Set triangle in the list. If the function return false this will be
  // overwritten
  triangles[index].a.set_values(pa.x(), pa.y());
  triangles[index].b.set_values(pb.x(), pb.y());
  triangles[index].c.set_values(pc.x(), pc.y());
  triangles[index].z_value = z_max;

  // 5. Check any point in camera bounds
  bool point_in_camera = is_point_between_camera_bounds(triangles[index].a)
                       | is_point_between_camera_bounds(triangles[index].b)
                       | is_point_between_camera_bounds(triangles[index].c);
  if (!point_in_camera) return false;

  // 6. Check normal of the face is towards camera, do not check angle,
  // only if it's bigger than 90º instead
  if (!double_faces) {
    bool angle_normal = (face.normal * face.a) < 0
                      | (face.normal * face.b) < 0
                      | (face.normal * face.c) < 0;
    if (!angle_normal) return false;
  }

  // 7. Calculate light contribution
  triangles[index].color = calculate_lights(color, face);
  return true;
}

void Rasteriser::rasterise() {
  set_rasterization_data();

  auto lambda = [&](unsigned init, unsigned end, unsigned vec_index) {
    Point3 a, b, c;
    unsigned i_from = 0, i_to = 0;

    for (unsigned j = init; j < end; j++) {
      Mesh* aux_mesh = meshes_vector[j];
      for (const auto& face : aux_mesh->global_coordenates_faces) {
        i_to += calculate_mesh_projection(face, projected_elements[vec_index],
                                          i_to, aux_mesh->color);

        // Copy triangles to elements_to_render vector
        if (((i_to - i_from) > 10 || i_to == (end - 1)) && mtx.try_lock()) {
          for (unsigned i = i_from; i < i_to; i++)
            elements_to_render.push_back(projected_elements[vec_index][i]);
          i_from = i_to;
          mtx.unlock();
        }
      }
    }
  };

  unsigned segments = (meshes_vector.size() / N_THREADS);

  std::vector<std::future<void>> promises (N_THREADS);
  for (unsigned i = 0; i < N_THREADS  - 1; i++)
    promises[i] = std::async(lambda, i * segments, (i + 1) * segments, i);

  promises[N_THREADS - 1] = std::async(lambda, (N_THREADS - 1) * segments,
                                               meshes_vector.size(),
                                               N_THREADS - 1);
  for (auto& promise : promises)
    promise.get();

  // Order by distance to camera
  std::sort(elements_to_render.begin(), elements_to_render.end(),
    [](const Triangle2& a, const Triangle2& b) {
      return a.z_value > b.z_value;
  });

  // FIXME: Calculate triangle occlusion here?
  canvas->update_frame(camera->get_bounds(), elements_to_render);
}

bool Rasteriser::is_point_between_camera_bounds(const Point2& p) const {
  return p.x() > camera->get_bounds().x       &&
         p.x() < camera->get_bounds().width   &&
         p.y() > camera->get_bounds().y       &&
         p.y() < camera->get_bounds().height;
}

// Calculate the intersection point between the camera plane and the vertex
/* Plane expresed as Ax + By + Cz + D = 0
 * Rect expresed as
 *  x = a + bß
 *  y = c + dß
 *  z = e + fß
 *
 * T1 = D + A*a + B*c + C*e
 * T2 = - A*b - B*d - C*f
 *
 * ß = T1 / T2
 *
 * */
bool Rasteriser::calculate_cut_point(const Point3& vertex,
                                     const Vector3& dir_v,
                                           Point3& point) const {  
  // Calc cut point line - plane
  const double A = camera->get_plane_vector().x();
  const double B = camera->get_plane_vector().y();
  const double C = camera->get_plane_vector().z();

  const double D = -(camera->get_plane_point().x() * A +
                     camera->get_plane_point().y() * B +
                     camera->get_plane_point().z() * C);

  const double& a = vertex.x();
  const double& c = vertex.y();
  const double& e = vertex.z();

  const double& b = dir_v.x();
  const double& d = dir_v.y();
  const double& f = dir_v.z();

  // NOTE: Changed from -D to avoid render upside down
  double T1 {D + A*a + B*c + C*e};
  double T2 {- A*b - B*d - C*f};  

  bool return_value = true;

  // Some vertices are behind camera
  if ((f < 0 && C > 0) || (f > 0 && C < 0)) {
    T1 = -T1;
    return_value = false;
  }  

  double parameter = T1 / T2;

  // Intersection in global coordiantes
  point.set_x(a + b * parameter);  
  point.set_y(-(c + d * parameter));
  point.set_z(e + f * parameter);

  return return_value;
}
