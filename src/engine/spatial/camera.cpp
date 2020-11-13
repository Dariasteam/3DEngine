#include "camera.h"

Camera::Camera(const Vector3& v_plane,
               const RectF& b) :
  bounds (b),
  vector_plane (v_plane),
  buffers(CommonBuffers::get())
{}

Camera::Camera(const Camera& cam) :
  vector_plane (cam.vector_plane),
  bounds (cam.bounds),
  point_plane (cam.point_plane),
  buffers(CommonBuffers::get())
{}

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


