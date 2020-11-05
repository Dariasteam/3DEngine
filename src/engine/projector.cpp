#include "projector.h"

Projector::Projector(Camera* cm, World* wd) :
  world (wd),
  camera (cm),
  elements_to_render(200000)
{
  for (unsigned i = 0; i < N_THREADS; i++) {
    tmp_triangles_sizes[i] = 0;
    tmp_triangles_i[i] = new Triangle2i[50000];
    tmp_triangles_f[i] = new Triangle2[50000];
  }
}

void Projector::generate_mesh_list(const std::vector<Mesh*> &meshes) {
  for (const auto& mesh : meshes) {
    meshes_vector.push_back(mesh);
    generate_mesh_list(mesh->get_nested_meshes());
  }
}


void Projector::multithreaded_rasterize_mesh_list(unsigned init, unsigned end) {
  n_elements_to_render = 0;
  for (unsigned i = init; i < end; i++) {
    Mesh* aux_mesh = meshes_vector[i];
    double segment_length = (double(aux_mesh->global_coordenates_faces.size()) / N_THREADS);

    c = 0;
    cv_bool = false;
    unsigned tmp_n_elements_to_render[N_THREADS];    

    auto& m = MultithreadManager::get_instance();
    m.calculate_threaded(N_THREADS, [&](unsigned i) {
      long unsigned from = i * segment_length;
      long unsigned to = (i + 1) * segment_length;                  

      multithreaded_rasterize_single_mesh(from,
                                          to,
                                          i,
                                          aux_mesh);

      tmp_n_elements_to_render[i] = tmp_triangles_sizes[i];
    });

    for (unsigned j = 0; j < N_THREADS; j++)
      n_elements_to_render += tmp_n_elements_to_render[j];
  }
}

void Projector::multithreaded_rasterize_single_mesh(unsigned init,
                                                    unsigned end,
                                                    unsigned index,
                                                    const Mesh* aux_mesh) {

  tmp_triangles_sizes[index] = 0;

  for (unsigned k = init; k < end; k++) {
    const auto& face = aux_mesh->global_coordenates_faces[k];
    const auto& uv   = aux_mesh->uv_per_face[k];
    calculate_mesh_projection(face, uv, index, aux_mesh->color);
  }  

  mtx.lock();
  c++;
  mtx.unlock();

  if (c == N_THREADS) {
    cv_bool = true;
    cv2.notify_all();
  } else {
    mtx.lock();
    std::unique_lock<std::mutex> lk(mtx2);
    mtx.unlock();

    cv2.wait(lk, [&]{return cv_bool;});
  }

  unsigned prev_offset = 0;

  for (unsigned i = 0; i < index; i++)
    prev_offset += tmp_triangles_sizes[i];

  std::copy(tmp_triangles_i[index],
            tmp_triangles_i[index] + tmp_triangles_sizes[index],
            std::begin(elements_to_render) +
                 prev_offset +
                 n_elements_to_render); // append if it's not the first element
}

void Projector::set_projection_data() {
  const auto& meshes = world->get_elements();  

  Vector3 camera_t;
  if (!camera->basis_changed)
    camera_t = camera->position;
  else
    camera_t = camera->translation;

  for (unsigned i = 0; i < meshes.size(); i++) {
    meshes[i]->express_in_parents_basis(camera->basis,
                                        camera_t,
                                        camera->basis_changed,
                                        camera->position_changed);
  }

  camera->basis_changed    = false;
  camera->position_changed = false;

  // Generate iterable list of meshes
  meshes_vector.clear();
  generate_mesh_list(world->get_elements());
  //elements_to_render.clear();
}

Color Projector::calculate_lights (const Color& m_color,
                                    const Vector3& normal) const {

  const DirectionalLight& light = world->get_light();

  // Using dot product as angle
  double angle_to_light = 1 + (normal * light.direction);

  Color color = light.color;
  color *= angle_to_light * world->get_light().intensity;

  color.set_x((color.x() * m_color.x() / 255));
  color.set_y((color.y() * m_color.y() / 255));
  color.set_z((color.z() * m_color.z() / 255));

  clamp_color(color);

  return color;
}

bool Projector::calculate_mesh_projection(const Face& face,
                                          const UV& uv,
                                          unsigned index,
                                          const Color& color) {

  auto& tmp_triangle = tmp_triangles_f[index][tmp_triangles_sizes[index]];

  //Triangle2 tmp_triangle;

  // ONLY USEFUL IF USING DOUBLE FACES
  // 1. Check face is not behind camera. Since we are using camera basis
  // we only need z value of the plane. Also each point coordinate is also
  // it's vector

  // FIXME: this should not exist
  /*
  auto plane_distance = camera->get_plane_point().z();  
  if (face.a.z() < plane_distance &&
      face.b.z() < plane_distance &&
      face.c.z() < plane_distance) return false;
      */

  // 2. Check normal of the face is towards camera, do not check angle,
  // only if it's bigger than 90º instead
  bool angle_normal = (face.normal * face.a) < 0
                    | (face.normal * face.b) < 0
                    | (face.normal * face.c) < 0;
  if (!angle_normal) { return false; }

  // 2. Calculate distance to camera
  double mod_v1 = Vector3::vector_module(face.a);
  double mod_v2 = Vector3::vector_module(face.b);
  double mod_v3 = Vector3::vector_module(face.c);

  double z_min = std::min({mod_v1, mod_v2, mod_v3});
  //double z_max = std::max({mod_v1, mod_v2, mod_v3});
  if (z_min > 10000) { return false; }

  // 3. Calculate intersection points with the plane
  bool visible  = calculate_cut_point(face.a, face.a, tmp_triangle.a)
                | calculate_cut_point(face.b, face.b, tmp_triangle.b)
                | calculate_cut_point(face.c, face.c, tmp_triangle.c);
  if (!visible) { return false; }

  tmp_triangle.z_value = z_min;  

  if (!triangle_inside_camera(tmp_triangle)) { return false; }
  // 7. Calculate light contribution for each vertex
/*
  Color aux_color = calculate_lights(color, face.normal);
  tmp_triangle.color   = Color888(aux_color);
*/
  tmp_triangle.a.color = calculate_lights(color, face.normal_a);
  tmp_triangle.b.color = calculate_lights(color, face.normal_b);
  tmp_triangle.c.color = calculate_lights(color, face.normal_c);

  // FIXME: This should be managed by the rasteriser
  // 8. Convert triangle to screen space
  auto& e = tmp_triangles_i[index][tmp_triangles_sizes[index]];
  triangle_to_screen_space(tmp_triangle, e);

  // Set uv
  e.uv = uv;
  tmp_triangles_sizes[index]++;

  return true;
}

void Projector::project() {
  set_projection_data();
  multithreaded_rasterize_mesh_list(0, meshes_vector.size());
}

bool Projector::is_point_between_camera_bounds(const Point2& p) const {
  return p.x() < camera->get_bounds().x       |
         p.x() > camera->get_bounds().width   |
         p.y() < camera->get_bounds().y       |
         p.y() > camera->get_bounds().height;
}

// Calculate the intersection point between the camera plane and the
// line described by vertex and camera_fugue (dir_v)
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
bool Projector::calculate_cut_point(const Point3& vertex,
                                    const Vector3& dir_v,
                                          Point2& point) {
  // Calc cut point line - plane
  const double A = camera->get_plane_vector().x();  // Since we are in camera space this should be always (0, 0, 1)
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
  double T1 {-D + A*a + B*c + C*e};
  double T2 {- A*b - B*d - C*f};  

  bool return_value = true;

  // Some vertices are behind camera
  if ((f < 0 && C > 0) || (f > 0 && C < 0)) {
    T1 = -T1;
    return_value = false;
  }

  double parameter = T1 / T2;

  // Intersection in global coordiantes FIXME: Should be in camera
  point.set_x(a + b * parameter);
  point.set_y(-(c + d * parameter));
  //point.set_z(e + f * parameter);

  return return_value;
}

unsigned screen_size = 1000;

bool Projector::triangle_inside_screen(const Triangle2i &triangle) {
  // Check all points inside render area
  if (triangle.a.x() < 0.0 || triangle.a.x() > screen_size) return false;
  if (triangle.a.y() < 0.0 || triangle.a.y() > screen_size) return false;
  if (triangle.b.x() < 0.0 || triangle.b.x() > screen_size) return false;
  if (triangle.b.y() < 0.0 || triangle.b.y() > screen_size) return false;
  if (triangle.c.x() < 0.0 || triangle.c.x() > screen_size) return false;
  if (triangle.c.y() < 0.0 || triangle.c.y() > screen_size) return false;

  return true;
}

bool Projector::triangle_inside_camera(const Triangle2 &triangle) {
  // Check all points inside render area
  if (triangle.a.x() < camera->get_bounds().x || triangle.a.x() > camera->get_bounds().width) return false;
  if (triangle.a.y() < camera->get_bounds().y || triangle.a.y() > camera->get_bounds().height) return false;
  if (triangle.b.x() < camera->get_bounds().x || triangle.b.x() > camera->get_bounds().width) return false;
  if (triangle.b.y() < camera->get_bounds().y || triangle.b.y() > camera->get_bounds().height) return false;
  if (triangle.c.x() < camera->get_bounds().x || triangle.c.x() > camera->get_bounds().width) return false;
  if (triangle.c.y() < camera->get_bounds().y || triangle.c.y() > camera->get_bounds().height) return false;

  return true;
}

// FIXME: Don not return anything
Triangle2i Projector::triangle_to_screen_space (const Triangle2& triangle, Triangle2i& t) {

  unsigned height = screen_size;
  unsigned width = screen_size;

  double v_factor = height / camera->get_bounds().y;
  double h_factor = width  / camera->get_bounds().x;

  unsigned x_offset = width  / 2;
  unsigned y_offset = height / 2;

  t.a.set_x(triangle.a.x() * h_factor + x_offset);
  t.a.set_y(triangle.a.y() * v_factor + y_offset);

  t.b.set_x(triangle.b.x() * h_factor + x_offset);
  t.b.set_y(triangle.b.y() * v_factor + y_offset);

  t.c.set_x(triangle.c.x() * h_factor + x_offset);
  t.c.set_y(triangle.c.y() * v_factor + y_offset);

  t.a.color = triangle.a.color;
  t.b.color = triangle.b.color;
  t.c.color = triangle.c.color;

  t.z_value = triangle.z_value;

  return t;
}
