#include "camera.h"

Camera::Camera(const Vector3& v_plane,
               const RectF& b) :
  local_vector_plane (v_plane),
  bounds (b),
  buffers(CommonBuffers::get())
{

}

Camera::Camera(const Camera& cam) :
  local_vector_plane (cam.local_vector_plane),
  bounds (cam.bounds),
  local_point_plane (cam.local_point_plane),
  buffers(CommonBuffers::get())
{}

void Camera::project(const std::vector<Mesh*> meshes_vector) const {
  /*
  unsigned n_faces = 0;
  for (const auto* mesh : meshes_vector) {
    n_faces += mesh->global_coordenates_faces.size();
  }
  */

  // FIXME: Use n_faces instead and compact all triangles at the begining
  // of buffer.triangles? Using that we could reduce the vector of TextureProjectors
  double regular_segment = double(buffers.triangles.size()) / N_THREADS;
  std::vector<unsigned> relevant_triangle_indices[N_THREADS];

  auto& m = MultithreadManager::get_instance();
  m.calculate_threaded(N_THREADS, [&](unsigned thread_indx) {
    unsigned triangle_index = std::round(regular_segment * thread_indx);

    for (const auto* mesh : meshes_vector) {
      const auto& faces = mesh->faces;

      double local_segment = double(faces.size()) / N_THREADS;

      unsigned from = std::round(local_segment * thread_indx);
      unsigned to = std::round(local_segment   * (thread_indx + 1));

      for (unsigned j = from; j < to; j++) {
        bool b = calculate_face_projection(faces[j],
                                           mesh->uv_per_face[j],
                                           triangle_index);
        if (b)
          relevant_triangle_indices[thread_indx].push_back(triangle_index);

        triangle_index++;
      }
    }
  });

  // Create a continuos vector of relevant indices

  buffers.n_renderable_triangles = 0;

  auto it = buffers.triangle_indices.begin();
  for (unsigned i = 0; i < N_THREADS; i++) {
    std::copy(relevant_triangle_indices[i].begin(),
              relevant_triangle_indices[i].end(),
              it);

    it += relevant_triangle_indices[i].size();
    buffers.n_renderable_triangles += relevant_triangle_indices[i].size();
  }
}
