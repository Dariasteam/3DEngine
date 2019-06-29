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

double Rasteriser::get_y (const Point2& u, const Vector2& v, double x) {
  return u.y() + (x - u.x()) * (v.y() / v.x());
}

double Rasteriser::get_x (const Point2& u, const Vector2& v, double y) {
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
  Point3 a, b, c;
  std::vector <Triangle2> tmp_triangles;

  for (unsigned k = init; k < end; k++) {
    const auto& face = aux_mesh->global_coordenates_faces[k];
    calculate_mesh_projection(face, tmp_triangles, aux_mesh->color);
  }

  mtx.lock();
  elements_to_render.insert(std::end(elements_to_render), std::begin(tmp_triangles), std::end(tmp_triangles));
  mtx.unlock();
}
/*
void Rasteriser::paint_line (const Point2& a, const Point2& b, ) {

  Vector2 v;
  Point2  u = a;

  // Calculate y knowing x

  if (a.x() > b.x())
    v = a - b;
  else
    v = b - a;

  uint init = static_cast<uint>(std::round((a.x())));
  uint end =  static_cast<uint>(std::floor((b.x())));

  for (unsigned x = init; x < end; x++) {
    uint y = static_cast<uint>(std::round(get_y(u, v, x)));
    screen_buffer[y][x] = {255, 255, 255};
  }

  // Calculate x knowing y

  if (a.y() > b.y())
    v = a - b;
  else
    v = b - a;

  init = static_cast<uint>(std::round((a.y())));
  end  = static_cast<uint>(std::floor((b.y())));

  for (unsigned y = init; y < end; y++) {
    uint x = static_cast<uint>(std::round(get_x(u, v, y)));
    screen_buffer[y][x] = {255, 255, 255};
  }
}
*/
/*
void Rasteriser::raster_triangle_y (const Triangle2& triangle) {

  const Point2& a = triangle.a;
  const Point2& b = triangle.b;
  const Point2& c = triangle.c;

  Point2 min_y_p;
  Point2 mid_y_p;
  Point2 max_y_p;

  bool p_a = false;
  bool p_b = false;
  bool p_c = false;

  if (a.y() <= b.y() && a.y() <= c.y()) { min_y_p = a; p_a = true; }
  if (b.y() <= a.y() && b.y() <= c.y()) { min_y_p = b; p_b = true; }
  if (c.y() <= a.y() && c.y() <= b.y()) { min_y_p = c; p_c = true; }

  if (a.y() >= b.y() && a.y() >= c.y()) { max_y_p = a; p_a = true; }
  if (b.y() >= a.y() && b.y() >= c.y()) { max_y_p = b; p_b = true; }
  if (c.y() >= a.y() && c.y() >= b.y()) { max_y_p = c; p_c = true; }

  if (!p_a) mid_y_p = a;
  if (!p_b) mid_y_p = b;
  if (!p_c) mid_y_p = c;

  // Now vertices are ordered from top to bottom

  Point2 min_x;
  Point2 max_x;

  if (mid_y_p.x() < max_y_p.x()) {
    min_x = mid_y_p;
    max_x = max_y_p;
  } else {
    min_x = max_y_p;
    max_x = mid_y_p;
  }

  unsigned y_min = static_cast<unsigned>(std::round(min_y_p.y()));
  unsigned y_max = static_cast<unsigned>(std::floor(mid_y_p.y()));

  for (unsigned y = y_min; y < y_max; y++) {
    int x_min = static_cast<int>(std::round(get_x(min_y_p, min_y_p - min_x, y)));
    int x_max = static_cast<int>(std::floor(get_x(min_y_p, min_y_p - max_x, y)));

    for (unsigned x = x_min; x <= x_max; x++) {
      screen_buffer[y][x] = {static_cast<unsigned>(triangle.color.x()),
                             static_cast<unsigned>(triangle.color.y()),
                             static_cast<unsigned>(triangle.color.z())
                            };
    }
  }

  if (mid_y_p.x() < min_y_p.x()) {
    min_x = mid_y_p;
    max_x = min_y_p;
  } else {
    min_x = min_y_p;
    max_x = mid_y_p;
  }

  y_min = static_cast<unsigned>(std::round(mid_y_p.y()));
  y_max = static_cast<unsigned>(std::floor(max_y_p.y()));

  for (unsigned y = y_max; y > y_min; y--) {
    int x_min = static_cast<int>(std::round(get_x(max_y_p, max_y_p - min_x, y)));
    int x_max = static_cast<int>(std::floor(get_x(max_y_p, max_y_p - max_x, y)));

    for (unsigned x = x_min; x <= x_max; x++) {
      screen_buffer[y][x] = {static_cast<unsigned>(triangle.color.x()),
                             static_cast<unsigned>(triangle.color.y()),
                             static_cast<unsigned>(triangle.color.z())
                            };
    }
  }
}
*/
/*
void Rasteriser::raster_triangle_x (const Triangle2& triangle) {

  const Point2& a = triangle.a;
  const Point2& b = triangle.b;
  const Point2& c = triangle.c;

  Point2 min_x_p;
  Point2 mid_x_p;
  Point2 max_x_p;

  bool p_a = false;
  bool p_b = false;
  bool p_c = false;

  if (a.x() <= b.x() && a.x() <= c.x()) { min_x_p = a; p_a = true; }
  if (b.x() <= a.x() && b.x() <= c.x()) { min_x_p = b; p_b = true; }
  if (c.x() <= a.x() && c.x() <= b.x()) { min_x_p = c; p_c = true; }

  if (a.x() >= b.x() && a.x() >= c.x()) { max_x_p = a; p_a = true; }
  if (b.x() >= a.x() && b.x() >= c.x()) { max_x_p = b; p_b = true; }
  if (c.x() >= a.x() && c.x() >= b.x()) { max_x_p = c; p_c = true; }

  if (!p_a) mid_x_p = a;
  if (!p_b) mid_x_p = b;
  if (!p_c) mid_x_p = c;

  // Now vertices are ordered from left to right

  Point2 min_y;
  Point2 max_y;

  if (mid_x_p.y() < max_x_p.y()) {
    min_y = mid_x_p;
    max_y = max_x_p;
  } else {
    min_y = max_x_p;
    max_y = mid_x_p;
  }

  unsigned x_min = static_cast<unsigned>(std::round(min_x_p.x()));
  unsigned x_max = static_cast<unsigned>(std::floor(mid_x_p.x()));

  for (unsigned x = x_min; x < x_max; x++) {
    int y_min = static_cast<int>(std::round(get_y(min_x_p, min_x_p - min_y, x)));
    int y_max = static_cast<int>(std::floor(get_y(min_x_p, min_x_p - max_y, x)));

    for (unsigned y = y_min; y <= y_max; y++) {
      screen_buffer[y][x] = {static_cast<unsigned>(triangle.color.x()),
                             static_cast<unsigned>(triangle.color.y()),
                             static_cast<unsigned>(triangle.color.z())
                            };
    }
  }

  if (mid_x_p.y() < max_x_p.y()) {
    min_y = mid_x_p;
    max_y = min_x_p;
  } else {
    min_y = min_x_p;
    max_y = mid_x_p;
  }

  x_min = static_cast<unsigned>(std::round(mid_x_p.x()));
  x_max = static_cast<unsigned>(std::floor(max_x_p.x()));

  for (unsigned x = x_max; x > x_min; x--) {
    int y_min = static_cast<int>(std::round(get_y(max_x_p, max_x_p - min_y, x)));
    int y_max = static_cast<int>(std::floor(get_y(max_x_p, max_x_p - max_y, x)));

    for (unsigned y = y_min; y <= y_max; y++) {
      screen_buffer[y][x] = {static_cast<unsigned>(triangle.color.x()),
                             static_cast<unsigned>(triangle.color.y()),
                             static_cast<unsigned>(triangle.color.z())
                            };
    }
  }
}
*/

void Rasteriser::raster_triangle(const Triangle2& triangle,
                                 std::vector<std::vector<Color888>>* screen_buffer) {
  // Generate a rectangle that envolves the triangle
  const double left  = std::min ({triangle.a.x(), triangle.b.x(), triangle.c.x()});
  const double right = std::max ({triangle.a.x(), triangle.b.x(), triangle.c.x()});

  const double top  = std::min ({triangle.a.y(), triangle.b.y(), triangle.c.y()});
  const double bttm = std::max ({triangle.a.y(), triangle.b.y(), triangle.c.y()});

  const unsigned l = std::max(static_cast<unsigned>(std::round(left)),  unsigned(0));
  const unsigned r = std::min(static_cast<unsigned>(std::round(right)), screen_size - 1);
  const unsigned t = std::max(static_cast<unsigned>(std::round(top)),   unsigned(0));
  const unsigned b = std::min(static_cast<unsigned>(std::round(bttm)),  screen_size - 1);

  // Use barycentric coordinates, iterate over every pixel inside the
  // rect and check if it belongs to the triangle or not
  Vector2 v0 = triangle.c - triangle.a;
  Vector2 v1 = triangle.b - triangle.a;

  for (unsigned x = l; x <= r; x++) {
    for (unsigned y = t; y <= b; y++) {
      Vector2 v2 = Point2(x,y) - triangle.a;
      // Compute vectors

      // Compute dot products
      double dot00 = v0 * v0;
      double dot01 = v0 * v1;
      double dot02 = v0 * v2;
      double dot11 = v1 * v1;
      double dot12 = v1 * v2;

      // Compute barycentric coordinates
      double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
      double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
      double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

      // Check if point is in triangle
      if ((u >= 0) && (v >= 0) && (u + v < 1) && triangle.z_value < z_buffer[y][x]) {
        (*screen_buffer)[y][x] = {static_cast<unsigned char>(triangle.color.x()),
                                  static_cast<unsigned char>(triangle.color.y()),
                                  static_cast<unsigned char>(triangle.color.z())};
        z_buffer[y][x] = triangle.z_value;
      }
    }
  }
}

void Rasteriser::paint_triangle (const Triangle2& triangle,
                                 std::vector<std::vector<Color888>>* screen_buffer) {

  unsigned height = screen_buffer->size();
  unsigned width = (*screen_buffer)[0].size();

  // FIXME: Use actual camera values
  double v_factor = height / 6;
  double h_factor = width  / 6;

  unsigned x_offset = width / 2;
  unsigned y_offset = height / 2;

  Point2 a = {triangle.a.x() * h_factor + x_offset,
              triangle.a.y() * v_factor + y_offset};

  Point2 b = {triangle.b.x() * h_factor + x_offset,
              triangle.b.y() * v_factor + y_offset};

  Point2 c = {triangle.c.x() * h_factor + x_offset,
              triangle.c.y() * v_factor + y_offset};

//  paint_line(a, b);
//  paint_line(b, c);
//  paint_line(a, c);

//  raster_triangle_y(Triangle2{a, b, c, triangle.z_value, triangle.color});
//  raster_triangle_x(Triangle2{a, b, c, triangle.z_value, triangle.color});

  raster_triangle (Triangle2{a, b, c, triangle.z_value, triangle.color}, screen_buffer);
}

void Rasteriser::generate_frame() {  
  std::vector<std::vector<Color888>>* buff;

  // 1. Select unused buffer
  canvas->lock_buffer_mutex();
  if (canvas->reading_from_buffer_a())
    buff = &screen_buffer_b;
  else
    buff = &screen_buffer_a;  

  // FIXME: Too much cost cleaning theese buffers
  // 2. Clear buffers
  std::fill(buff->begin(), buff->end(),
            std::vector<Color888>(screen_size, {0,0,0}));

  std::fill(z_buffer.begin(), z_buffer.end(),
            std::vector<double>(screen_size, 100000));

  // 3. Populate
  unsigned size = elements_to_render.size();
  unsigned segments = (size / N_THREADS);

  auto lambda = [&](unsigned init, unsigned end) {
    for (unsigned j = init; j < end; j++)
      paint_triangle(elements_to_render[j], buff);
  };

  std::vector<std::future<void>> promises (N_THREADS);
  for (unsigned i = 0; i < N_THREADS - 1; i++)
    promises[i] = std::async(lambda, i * segments, (i + 1) * segments);

  promises[N_THREADS - 1] = std::async(lambda, (N_THREADS - 1) * segments, size);

  for (auto& promise : promises)
    promise.get();

  canvas->unlock_buffer_mutex();                 // Acts like Vsync
  canvas->update_frame(camera->get_bounds());
//  write_file(screen_buffer);
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

  color.set_x(std::min (fabs(color.x() * m_color.x() / 255), 254.0));
  color.set_y(std::min (fabs(color.y() * m_color.y() / 255), 254.0));
  color.set_z(std::min (fabs(color.z() * m_color.z() / 255), 254.0));

  return color;
}

bool Rasteriser::calculate_mesh_projection(const Face3& face,                                           
                                           std::vector<Triangle2>& triangles,                                           
                                           const Color& color) {
  Triangle2 tmp_triangle;

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

  // 7. Calculate light contribution
  tmp_triangle.color = calculate_lights(color, face);
  triangles.push_back(tmp_triangle);
  return true;
}

void Rasteriser::rasterise() {
  set_rasterization_data();    

  // Prepare threads
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

  // Order by distance to camera
  // EDIT: Do not order since we are using a multithreaded painting
  /*
  std::sort(buff->begin(), buff->end(),
    [](const Triangle2& a, const Triangle2& b) {
      return a.z_value > b.z_value;
  });
  */

  // FIXME: Calculate triangle occlusion here?
  // EDIT: Currently calculated using a z_buffer
  generate_frame();
}

bool Rasteriser::is_point_between_camera_bounds(const Point2& p) const {
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
