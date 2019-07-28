#include "rasteriser.h"

Rasteriser::Rasteriser(Canvas* cv, Camera* cm, World* wd) :
  world (wd),
  camera (cm),
  canvas (cv),
  screen_buffer_a (screen_size, std::vector<Color888>(screen_size, {0,0,0})),
  screen_buffer_b (screen_size, std::vector<Color888>(screen_size, {0,0,0})),
  z_buffer (screen_size, std::vector<double>(screen_size, 10000))
{
  canvas->set_screen_buffers(&screen_buffer_a, &screen_buffer_b);
}

void Rasteriser::generate_mesh_list(const std::vector<Mesh*> &meshes) {
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

double Rasteriser::get_y (const Point2F& u, const Vector2& v, double x) {
  return u.y() + (x - u.x()) * (v.y() / v.x());
}

double Rasteriser::get_x (const Point2F& u, const Vector2& v, double y) {
  return u.x() + (y - u.y()) * (v.x() / v.y());
}

void Rasteriser::multithreaded_rasterize_mesh_list(unsigned init, unsigned end) {
  for (unsigned i = init; i < end; i++) {
    Mesh* aux_mesh = meshes_vector[i];
    unsigned segments = (aux_mesh->global_coordenates_faces.size() / N_THREADS);
    std::vector<std::future<void>> promises_2 (N_THREADS);

    for (unsigned j = 0; j < N_THREADS  - 1; j++)
      promises_2[j] = std::async(&Rasteriser::multithreaded_rasterize_single_mesh,
                                 this, j * segments, (j + 1) * segments, aux_mesh);

    promises_2[N_THREADS - 1] = std::async(&Rasteriser::multithreaded_rasterize_single_mesh,
                                           this, (N_THREADS - 1) * segments,
                                           meshes_vector.size(), aux_mesh);
    for (auto& promise : promises_2)
      promise.get();
  }
}

void Rasteriser::multithreaded_rasterize_single_mesh(unsigned init, unsigned end,                                                     
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

void drawLine (unsigned x0, unsigned x1, unsigned y, Color888 color,
               std::vector<std::vector<Color888>>* screen_buffer) {
  for (unsigned i = x0; i < x1; i++)
    (*screen_buffer)[y][i] = color;

}

inline void clamp_color (Color& color) {
  color.set_x(std::max (0.0, std::min(color.x(), 255.0)));
  color.set_y(std::max (0.0, std::min(color.y(), 255.0)));
  color.set_z(std::max (0.0, std::min(color.z(), 255.0)));
}

inline Color get_color_at_position_in_grading (const Color& color1,
                                               const Color& color2,
                                               double p1,
                                               double p2,
                                               double pos) {

  if (p1 > p2)
    std::swap(p1, p2);

  // now p1 < pos < p2

  double r_ratio = double(color2.x() - color1.x()) / double(p2 - p1);
  double g_ratio = double(color2.y() - color1.y()) / double(p2 - p1);
  double b_ratio = double(color2.z() - color1.z()) / double(p2 - p1);

  double new_r = color1.x() + (r_ratio * (pos - p1));
  double new_g = color1.y() + (g_ratio * (pos - p1));
  double new_b = color1.z() + (b_ratio * (pos - p1));

  return Color (new_r, new_g, new_b);
}

/* Considering a triangle with the form
 *
 *  v1 _____v2
 *   \    /
 *    \  /
 *     v3
 *
 * First we calculate the linear grading color of the lines l1 = v3 - v1,
 * and l2 = v3 - v2
 *
 * Then in base of the Y of the point, we calculate the linear grading of
 * l3 = l1[y] - l2[y]
 *
 * Finally, we find the color of l3[y][x]
 *
 * */
inline Color888 calculate_color_top_flat (const Triangle2& triangle, int x, int y,
                                          int min_x, int max_x) {
  auto v1 = triangle.a;
  auto v2 = triangle.b;
  auto v3 = triangle.c;

  // grading between v3 and v1
  Color color_l1 = get_color_at_position_in_grading(v1.color,
                                                    v3.color,
                                                    v1.y(),
                                                    v3.y(),
                                                    y);

  // grading between v3 and v2
  Color color_l2 = get_color_at_position_in_grading(v2.color,
                                                    v3.color,
                                                    v2.y(),
                                                    v3.y(),
                                                    y);

  Color final_color = get_color_at_position_in_grading(color_l1,
                                                       color_l2,
                                                       min_x,
                                                       max_x,
                                                       x);

  clamp_color(final_color);
  return Color888 (final_color);
}


void Rasteriser::fillBottomFlatTriangle(const Triangle2& triangle,
                            std::vector<std::vector<Color888>>* screen_buffer) {
  auto v1 = triangle.a;
  auto v2 = triangle.b;
  auto v3 = triangle.c;

  if (v3.y() == v1.y()) return; // we already know v2 = v3, triangle would be a line

  if (v2.x() > v3.x()) // order from left to right
    std::swap(v2, v3);  

  double invslope1 = double(v2.x()  - v1.x()) / (v2.y() - v1.y());
  double invslope2 = double(v3.x()  - v1.x()) / (v3.y() - v1.y());

  double curx1 = v1.x();
  double curx2 = curx1;

  int y1 = v1.y();
  int y2 = v2.y();

  for (int y = y1; y <= y2; y++) {
    for (int x = static_cast<int>(std::round(curx1)); x <= static_cast<int>(std::round(curx2)); x++) {
      if (triangle.z_value < z_buffer[y][x]) {
        (*screen_buffer)[y][x] = Color888 (triangle.color);
                z_buffer[y][x] = triangle.z_value;
      }
    }
    curx1 += invslope1;
    curx2 += invslope2;
  }
}

void Rasteriser::fillTopFlatTriangle(const Triangle2& triangle,
                            std::vector<std::vector<Color888>>* screen_buffer) {
  auto v1 = triangle.a;
  auto v2 = triangle.b;
  auto v3 = triangle.c;

  if (v3.y() == v1.y()) return; // we already know v2 = v1, triangle would be a line

  if (v1.x() > v2.x()) // order from left to right
    std::swap(v1, v2);

  double invslope1 = double(v3.x() - v1.x()) / (v3.y() - v1.y());
  double invslope2 = double(v3.x() - v2.x()) / (v3.y() - v2.y());

  double curx1 = v3.x();
  double curx2 = curx1;

  int y3 = v3.y();
  int y1 = v1.y();

  for (int y = y3; y >= y1; y--) {
    int min_x = static_cast<int>(std::round(curx1));
    int max_x = static_cast<int>(std::round(curx2));
    for (int x = min_x; x <= max_x; x++) {

      Color888 aux = calculate_color_top_flat (triangle, x, y, min_x, max_x);

      if (triangle.z_value < z_buffer[y][x]) {
//        (*screen_buffer)[y][x] = Color888 (triangle.color);
        (*screen_buffer)[y][x] = aux;
                z_buffer[y][x] = triangle.z_value;
      }
    }
    curx1 -= invslope1;
    curx2 -= invslope2;
  }
}

inline bool is_equal (double a, double b) {
  return std::isless(std::abs(a - b), 0.00001);
}

void Rasteriser::rasterize_triangle (Triangle2& triangle,
                                std::vector<std::vector<Color888>>* screen_buffer) {

  // Sort vertices by Y
  std::vector<Vertex2> aux_vec = {triangle.a, triangle.b, triangle.c};
  std::sort (aux_vec.begin(), aux_vec.end(), [&](const Vertex2& a, const Vertex2& b) {
    return std::isless(a.y(), b.y());
  });

  triangle.a = aux_vec[0];
  triangle.b = aux_vec[1];
  triangle.c = aux_vec[2];

  const Vertex2& v1 = aux_vec[0];
  const Vertex2& v2 = aux_vec[1];
  const Vertex2& v3 = aux_vec[2];

  // aux_triangle is ordered by y (v1 < v2 < v3)
  if (v2.y() == v3.y()) {
    fillBottomFlatTriangle(triangle, screen_buffer);
  } else if (v1.y() == v2.y()) {
    fillTopFlatTriangle(triangle, screen_buffer);
  } else {

    double a = v3.y() - v1.y();
    double b = v3.x() - v1.x();

    double ratio = b / a;
    double x = v1.x() + (ratio * (v2.y() - v1.y()));

    Vertex2 v4 (static_cast<int>(std::round(x)), v2.y());
    v4.color = get_color_at_position_in_grading(v1.color,
                                                v3.color,
                                                v1.y(),
                                                v3.y(),
                                                v4.y());

    Triangle2 aux_t1 {triangle};
    Triangle2 aux_t2 {triangle};

    aux_t1.c = v4;

    aux_t2.a = v2;
    aux_t2.b = v4;

    fillBottomFlatTriangle (aux_t1, screen_buffer);
    fillTopFlatTriangle    (aux_t2, screen_buffer);
  }
}

bool Rasteriser::triangle_inside_screen(const Triangle2 &triangle) {
  // Check all points inside render area
  if (triangle.a.x() < 0.0 || triangle.a.x() > screen_size) return false;
  if (triangle.a.y() < 0.0 || triangle.a.y() > screen_size) return false;
  if (triangle.b.x() < 0.0 || triangle.b.x() > screen_size) return false;
  if (triangle.b.y() < 0.0 || triangle.b.y() > screen_size) return false;
  if (triangle.c.x() < 0.0 || triangle.c.x() > screen_size) return false;
  if (triangle.c.y() < 0.0 || triangle.c.y() > screen_size) return false;

  return true;
}

Triangle2 Rasteriser::triangle_to_screen_space (const Triangle2F& triangle) {

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

void Rasteriser::generate_frame() {  
  std::vector<std::vector<Color888>>* buff;

  // 1. Select unused buffer
  canvas->lock_buffer_mutex();
  if (canvas->reading_from_buffer_a())
    buff = &screen_buffer_b;
  else
    buff = &screen_buffer_a;  

  // 2. Clear buffers
  std::fill(buff->begin(), buff->end(),
            std::vector<Color888>(screen_size, {0,0,0}));

  std::fill(z_buffer.begin(), z_buffer.end(),
            std::vector<double>(screen_size, 100000));

  // 3. Rasterize
  auto& m = MultithreadManager::get_instance();
  m.calculate_threaded(elements_to_render.size(), [&](unsigned i) {
    rasterize_triangle(elements_to_render[i], buff);
  });

  canvas->unlock_buffer_mutex();                 // Acts like Vsync
  canvas->update_frame(camera->get_bounds());
//  write_file(screen_buffer);
}

void Rasteriser::set_rasterization_data() {
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

Color Rasteriser::calculate_lights (const Color& m_color,
                                    const Face3& face) const {

  const DirectionalLight& light = world->get_light();

  // Using dot product as angle
  double angle_to_light = 1 + (face.normal * light.direction);

  Color color = light.color;
  color *= angle_to_light * world->get_light().intensity;;  

  color.set_x((color.x() * m_color.x() / 255));
  color.set_y((color.y() * m_color.y() / 255));
  color.set_z((color.z() * m_color.z() / 255));

  clamp_color(color);

  return color;
}

bool Rasteriser::calculate_mesh_projection(const Face3& face,                                           
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
  Color aux_color = calculate_lights(color, face);
  tmp_triangle.color   = Color888(aux_color);
  tmp_triangle.a.color = aux_color;
  tmp_triangle.b.color = aux_color;
  tmp_triangle.c.color = aux_color;
//  tmp_triangle.b.color = {0, 200, 100};
  tmp_triangle.c.color = {0, 0, 250};

  // 8. Convert triangle to screen space
  const Triangle2 final_triangle = triangle_to_screen_space(tmp_triangle);

  // 9. Check triangle inside screen area
  if (!triangle_inside_screen(final_triangle)) return false;

  // 10. Insert triangle
  triangles.push_back(final_triangle);

  return true;
}

void Rasteriser::rasterise() {
  set_rasterization_data();    

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
    promises[i] = std::async(&Rasteriser::multithreaded_rasterize_mesh_list, this,
                             i * segments, (i + 1) * segments);

  promises[N_THREADS - 1] = std::async(&Rasteriser::multithreaded_rasterize_mesh_list, this,
                                       (N_THREADS - 1) * segments, meshes_vector.size());

  // Trigger calculations
  for (auto& promise : promises)
    promise.get();

  generate_frame();
}

bool Rasteriser::is_point_between_camera_bounds(const Point2F& p) const {
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
