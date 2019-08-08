#include "projector.h"

Projector::Projector(Camera* cm, World* wd) :
  world (wd),
  camera (cm)
{}

void Projector::generate_mesh_list(const std::vector<Mesh*> &meshes) {
  for (const auto& mesh : meshes) {
    meshes_vector.push_back(mesh);
    generate_mesh_list(mesh->get_nested_meshes());
  }
}

void write_file(const std::vector<std::vector<Color888>>& canvas) {
  std::ofstream file;
  file.open("test_picture.ppm");

  if (file.is_open()) {
    file << "P3\n" << canvas[0].size() << " " << canvas.size() << " " << 255 << "\n";
    for (const auto &row : canvas) {
      for (const auto &pixel : row) {
        file << pixel.r << " "
             << pixel.g << " "
             << pixel.b << "\n";
      }
    }
    file.close();
  } else {
    std::cerr << "Could not create picture" << std::endl;
  }
}

double Projector::get_y (const Point2F& u, const Vector2& v, double x) {
  return u.y() + (x - u.x()) * (v.y() / v.x());
}

double Projector::get_x (const Point2F& u, const Vector2& v, double y) {
  return u.x() + (y - u.y()) * (v.x() / v.y());
}

void Projector::multithreaded_rasterize_mesh_list(unsigned init, unsigned end) {
  for (unsigned i = init; i < end; i++) {
    Mesh* aux_mesh = meshes_vector[i];
    unsigned segments = (aux_mesh->global_coordenates_faces.size() / N_THREADS);
    std::vector<std::future<void>> promises_2 (N_THREADS);

    for (unsigned j = 0; j < N_THREADS  - 1; j++)
      promises_2[j] = std::async(&Projector::multithreaded_rasterize_single_mesh,
                                 this, j * segments, (j + 1) * segments, aux_mesh);

    promises_2[N_THREADS - 1] = std::async(&Projector::multithreaded_rasterize_single_mesh,
                                           this, (N_THREADS - 1) * segments,
                                           meshes_vector.size(), aux_mesh);
    for (auto& promise : promises_2)
      promise.get();
  }
}

void Projector::multithreaded_rasterize_single_mesh(unsigned init, unsigned end,
                                                     const Mesh* aux_mesh) {
  std::vector <Triangle2> tmp_triangles;

  for (unsigned k = init; k < end; k++) {
    const auto& face = aux_mesh->global_coordenates_faces[k];
    calculate_mesh_projection(face, tmp_triangles, aux_mesh->color);
  }

  mtx.lock();
  elements_to_render.insert(std::end(elements_to_render), std::begin(tmp_triangles), std::end(tmp_triangles));
  mtx.unlock();
}

void Projector::set_projection_data() {
  const auto& meshes = world->get_elements();  

  Vector3 camera_t;
  if (!camera->basis_changed)
    camera_t = camera->position;
  else
    camera_t = camera->translation;

  auto& m = MultithreadManager::get_instance();
  m.calculate_threaded(meshes_vector.size(), [&](unsigned i) {
    meshes[i]->express_in_parents_basis(camera->basis,
                                        camera_t,
                                        camera->basis_changed,
                                        camera->position_changed);
  });

  camera->basis_changed    = false;
  camera->position_changed = false;

  // Generate iterable list of meshes
  meshes_vector.clear();
  generate_mesh_list(world->get_elements());
  elements_to_render.clear();
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

bool Projector::calculate_mesh_projection(const Face3& face,
                                           std::vector<Triangle2>& triangles,
                                           const Color& color) {
  Triangle2F tmp_triangle;

  // 1. Check face is not behind camera. Since we are using camera basis
  // we only need z value of the plane. Also each point coordinate is also
  // it's vector
  auto plane_distance = camera->get_plane_point().z();
  if (face.a.z() < plane_distance &&
      face.b.z() < plane_distance &&
      face.c.z() < plane_distance) return false;

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

  // Set values
  tmp_triangle.a.set_values(pa.x(), pa.y());
  tmp_triangle.b.set_values(pb.x(), pb.y());
  tmp_triangle.c.set_values(pc.x(), pc.y());
  tmp_triangle.z_value = z_max;

  // 5. Check any point in camera bounds
  bool point_in_camera = is_point_between_camera_bounds(tmp_triangle.a)
                       | is_point_between_camera_bounds(tmp_triangle.b)
                       | is_point_between_camera_bounds(tmp_triangle.c);
  if (!point_in_camera) return false;

  // 6. Check normal of the face is towards camera, do not check angle,
  // only if it's bigger than 90º instead
  if (!double_faces) {
    bool angle_normal = (face.normal * face.a) < 0
                      | (face.normal * face.b) < 0
                      | (face.normal * face.c) < 0;
    if (!angle_normal) return false;
  }

  // 7. Calculate light contribution for each vertices
  Color aux_color = calculate_lights(color, face.normal);
  tmp_triangle.color   = Color888(aux_color);
  tmp_triangle.a.color = calculate_lights(color, face.normal_a);
  tmp_triangle.b.color = calculate_lights(color, face.normal_b);
  tmp_triangle.c.color = calculate_lights(color, face.normal_c);

  // FIXME: THIS

  // 8. Convert triangle to screen space
  const Triangle2 final_triangle = triangle_to_screen_space(tmp_triangle);

  // 9. Check triangle inside screen area
  if (!triangle_inside_screen(final_triangle)) return false;

  // 10. Insert triangle
  // FIXME: Changed form "final_triangle"
  triangles.push_back(final_triangle);

  return true;
}

std::vector<Triangle2>& Projector::project() {
  set_projection_data();

  // Prepare threads
/*
  auto& m = MultithreadManager::get_instance();
  m.calculate_threaded(meshes_vector.size(), [&](unsigned i) {
    Mesh* aux_mesh = meshes_vector[i];
    multithreaded_rasterize_single_mesh(aux_mesh);
  });
*/

  unsigned segments = (meshes_vector.size() / N_THREADS);
  std::vector<std::future<void>> promises (N_THREADS);
  for (unsigned i = 0; i < N_THREADS  - 1; i++)
    promises[i] = std::async(&Projector::multithreaded_rasterize_mesh_list, this,
                             i * segments, (i + 1) * segments);

  promises[N_THREADS - 1] = std::async(&Projector::multithreaded_rasterize_mesh_list, this,
                                       (N_THREADS - 1) * segments, meshes_vector.size());

  // Trigger calculations
  for (auto& promise : promises)
    promise.get();

  return elements_to_render;
//  generate_frame();
}

bool Projector::is_point_between_camera_bounds(const Point2F& p) const {
  return p.x() > camera->get_bounds().x       &&
         p.x() < camera->get_bounds().width   &&
         p.y() > camera->get_bounds().y       &&
         p.y() < camera->get_bounds().height;
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
  double T1 {-D + A*a + B*c + C*e};
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

unsigned screen_size = 1000;

bool Projector::triangle_inside_screen(const Triangle2 &triangle) {
  // Check all points inside render area
  if (triangle.a.x() < 0.0 || triangle.a.x() > screen_size) return false;
  if (triangle.a.y() < 0.0 || triangle.a.y() > screen_size) return false;
  if (triangle.b.x() < 0.0 || triangle.b.x() > screen_size) return false;
  if (triangle.b.y() < 0.0 || triangle.b.y() > screen_size) return false;
  if (triangle.c.x() < 0.0 || triangle.c.x() > screen_size) return false;
  if (triangle.c.y() < 0.0 || triangle.c.y() > screen_size) return false;

  return true;
}

Triangle2 Projector::triangle_to_screen_space (const Triangle2F& triangle) {

  unsigned height = screen_size;
  unsigned width = screen_size;

  double v_factor = height / camera->get_bounds().y;
  double h_factor = width  / camera->get_bounds().x;

  unsigned x_offset = width  / 2;
  unsigned y_offset = height / 2;

  Point2F a = {triangle.a.x() * h_factor + x_offset,
               triangle.a.y() * v_factor + y_offset};

  Point2F b = {triangle.b.x() * h_factor + x_offset,
               triangle.b.y() * v_factor + y_offset};

  Point2F c = {triangle.c.x() * h_factor + x_offset,
               triangle.c.y() * v_factor + y_offset};

  Triangle2 t {triangle};
  t.a = a;
  t.b = b;
  t.c = c;

  t.a.color = triangle.a.color;
  t.b.color = triangle.b.color;
  t.c.color = triangle.c.color;

  return t;
}
