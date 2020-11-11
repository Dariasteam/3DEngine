#include "projector.h"

Projector::Projector(PerspectiveCamera* cm, World* wd) :
  world (wd),
  camera (cm),
  buffers (CommonBuffers::get())
{
  buffers.triangles_size = 0;
  buffers.triangles.resize(50000);
}

void Projector::project() {
  set_projection_data();

  unsigned n_faces = 0;
  for (const auto* mesh : meshes_vector) {
    n_faces += mesh->local_coordenates_faces.size();
  }

  double regular_segment = double(n_faces) / N_THREADS;
  unsigned lengths [N_THREADS];

  auto& m = MultithreadManager::get_instance();
  m.calculate_threaded(N_THREADS, [&](unsigned i) {
    lengths[i] = 0;
    unsigned base_index = std::round(regular_segment * i);

    for (const auto* mesh : meshes_vector) {
      const auto& faces = mesh->global_coordenates_faces;

      double local_segment = double(faces.size()) / N_THREADS;

      unsigned long from = std::round(local_segment * i);
      unsigned long to = std::round(local_segment * (i + 1));

      for (unsigned j = from; j < to; j++) {
        bool b = calculate_mesh_projection(faces[j],
                                           mesh->uv_per_face[j],
                                           base_index + lengths[i]);
        if (b)
          lengths[i]++;
      }
    }
  });

  buffers.triangles_size = lengths[0];

  for (unsigned i = 1; i < N_THREADS; i++) {
    unsigned base_index = std::round(regular_segment * i);

    std::copy(buffers.triangles.begin() + base_index,
              buffers.triangles.begin() + base_index + lengths[i],
              buffers.triangles.begin() + buffers.triangles_size);

    buffers.triangles_size += lengths[i];
  }
}

void Projector::generate_mesh_list(const std::vector<Mesh*> &meshes) {
  for (const auto& mesh : meshes) {
    meshes_vector.push_back(mesh);
    generate_mesh_list(mesh->get_nested_meshes());
  }
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
}

bool Projector::calculate_mesh_projection(const Face& face,
                                          const UV& uv,
                                          unsigned index) {

  auto& tmp_triangle = buffers.triangles[index];

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
  if (!angle_normal) return false;

  // 2. Calculate distance to camera
  double mod_v1 = Vector3::vector_module(face.a);
  double mod_v2 = Vector3::vector_module(face.b);
  double mod_v3 = Vector3::vector_module(face.c);

  double z_min = std::min({mod_v1, mod_v2, mod_v3});
  //double z_max = std::max({mod_v1, mod_v2, mod_v3});  
  if (z_min > INFINITY_DISTANCE) return false;

  // 3. Calculate intersection points with the plane
  bool visible  = calculate_cut_point(face.a, face.a, tmp_triangle.a)
                | calculate_cut_point(face.b, face.b, tmp_triangle.b)
                | calculate_cut_point(face.c, face.c, tmp_triangle.c);
  if (!visible) return false;

  tmp_triangle.z_value = z_min;  

  if (!triangle_inside_camera(tmp_triangle)) return false;

  // 4. Copy normals FIXME: Use only vertex normals, not the global one
  tmp_triangle.normal   = face.normal;
  tmp_triangle.normal_a = face.normal_a;
  tmp_triangle.normal_b = face.normal_b;
  tmp_triangle.normal_c = face.normal_c;

  // Set uv
  tmp_triangle.uv = uv;

  //buffers.triangles_sizes[thread_index]++;
  return true;
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

bool Projector::triangle_inside_camera(const Triangle &triangle) {
  // Check all points inside render area
  if (triangle.a.x() < camera->get_bounds().x || triangle.a.x() > camera->get_bounds().width) return false;
  if (triangle.a.y() < camera->get_bounds().y || triangle.a.y() > camera->get_bounds().height) return false;
  if (triangle.b.x() < camera->get_bounds().x || triangle.b.x() > camera->get_bounds().width) return false;
  if (triangle.b.y() < camera->get_bounds().y || triangle.b.y() > camera->get_bounds().height) return false;
  if (triangle.c.x() < camera->get_bounds().x || triangle.c.x() > camera->get_bounds().width) return false;
  if (triangle.c.y() < camera->get_bounds().y || triangle.c.y() > camera->get_bounds().height) return false;

  return true;
}
