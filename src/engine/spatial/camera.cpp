#include "camera.h"

Camera::Camera(const Vector3& v_plane, const Point3& p_plane) :
  vector_plane (v_plane),
  point_plane (p_plane),
  buffers(CommonBuffers::get())
{}

Camera::Camera(const Camera& cam) :
  vector_plane (cam.vector_plane),
  bounds (cam.bounds),
  point_plane (cam.point_plane),
  buffers(CommonBuffers::get())
{}

inline bool Camera::is_point_between_camera_bounds(const Point2& p) const {
  return p.x() >= get_bounds().x       &
         p.x() <= get_bounds().width   &
         p.y() >= get_bounds().y       &
         p.y() <= get_bounds().height;
}

inline bool Camera::triangle_inside_camera(const Triangle &triangle) const {
  return is_point_between_camera_bounds(triangle.a) &
         is_point_between_camera_bounds(triangle.b) &
         is_point_between_camera_bounds(triangle.c);
}

void Camera::project(const std::vector<Mesh*> meshes_vector) const {
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

  // Relocate to make a continuos vector
  for (unsigned i = 1; i < N_THREADS; i++) {
    unsigned base_index = std::round(regular_segment * i);

    std::copy(buffers.triangles.begin() + base_index,
              buffers.triangles.begin() + base_index + lengths[i],
              buffers.triangles.begin() + buffers.triangles_size);

    buffers.triangles_size += lengths[i];
  }
}

inline bool Camera::calculate_mesh_projection(const Face& face,
                                              const UV& uv,
                                              unsigned index) const {

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
  return true;
}
