#include "mesh.h"

void Mesh::change_basis_part (const Matrix3& basis_changer,
                              const Vector3& translation,
                              unsigned from, unsigned to) {
  for (unsigned k = from; k < to; k++) {
    auto& face = global_coordenates_faces[k];
    for (unsigned j = 0; j < 3; j++) {
      Point3Ops::change_basis(basis_changer, face[j], face[j]);
      Point3Ops::change_basis(basis_changer, face.get_normal(j), face.get_normal(j));
      face[j] += translation;
    }
    Point3Ops::change_basis(basis_changer, face.normal, face.normal);
  }
}

// Multithreaded change basis
void Mesh::change_basis_multithreaded(const std::list<Mesh*>& mesh_list,
                                      const Basis3& new_basis,
                                      const Point3& pos) {

  Matrix3 camera_basis_changer;
  MatrixOps::generate_basis_change_matrix(basis, new_basis, camera_basis_changer);

  Vector3 aux_pos;
  Point3Ops::change_basis(new_basis, (position - pos), aux_pos);

  global_coordenates_faces = local_coordenates_faces;
  const auto& lambda = [&](Mesh* mesh, unsigned from, unsigned to) {
    mesh->change_basis_part(camera_basis_changer, aux_pos, from, to);
  };

  for (const auto& mesh : mesh_list) {
    unsigned size = mesh->global_coordenates_faces.size();
    double segment = double(size) / N_THREADS;

    auto& m = MultithreadManager::get_instance();
    m.calculate_threaded(N_THREADS, [&](unsigned i) {
      lambda (mesh, i * segment, (i + 1) * segment);
    });
  }
}

std::list<Mesh*> Mesh::express_in_parents_basis(const Basis3& new_basis,
                                                const Point3& camera_translation) {

  // Generates a list with this mesh and it's nested ones
  std::list<Mesh*> mesh_list {this};

  for (auto& nested_mesh : nested_meshes)
    mesh_list.splice(mesh_list.end(),
                     nested_mesh->express_in_parents_basis(new_basis,
                                                           camera_translation));

  change_basis_multithreaded (mesh_list,
                              new_basis,
                              camera_translation);

  return mesh_list;
}

std::vector<Normal3*> Mesh::get_adjacent_vertices(Point3& p,
                                                  unsigned from,
                                                  std::vector<bool>& vertex_normals) {
  std::vector<Normal3*> adjacents;

  for (unsigned i = from; i < local_coordenates_faces.size(); i++) {
    for (unsigned j = 0; j < 3; j++) {
      if (!vertex_normals[i * 3 + j] && local_coordenates_faces[i][j] == p) {
        adjacents.push_back(&local_coordenates_faces[i].get_normal(j));
        vertex_normals[i * 3 + j] = true;
      }
    }
  }
  return adjacents;
}

void Mesh::generate_data() {
  generate_normals();

  std::vector<bool> vertex_normals (local_coordenates_faces.size() * 3, false);

  std::vector<Point3>  current_point_per_thread (N_THREADS);
  std::mutex mtx;
  unsigned last_index = 0;

  // Calculate interploted normals of the Vertices
  auto lambda = [&](unsigned i_thread) {
    unsigned init = last_index;
    bool end = false;

    // find next point not used by another thread
    do {
      mtx.lock();
      init = last_index;
      do {
        for (int j = 0; j < 3; j++) {
          for (int k = 0; k < current_point_per_thread.size(); k++) {
            end = true;
            if (vertex_normals[init * 3 + j] || current_point_per_thread[k] == local_coordenates_faces[init][j]) {
              end = false;
              break;
            }
          }
          if (end) {
            vertex_normals[init * 3 + j] = true;
            current_point_per_thread[i_thread] = local_coordenates_faces[init][j];
            break;
          }
        }
        if (!end)
          init++;
      } while (!end && init < local_coordenates_faces.size());

      last_index = init;
      std::cout << '\r' << 100 * double(init) / double(local_coordenates_faces.size()) << " % " << std::flush;
      mtx.unlock();

      Point3 point = local_coordenates_faces[init][0];
      Vector3 p_normal = local_coordenates_faces[init].normal.toVector3();
      auto adjacents = get_adjacent_vertices(point, ++init, vertex_normals);

      for (Normal3* aux_p : adjacents) {
        p_normal += aux_p->toVector3();
      }

      //p_normal /= (adjacents.size());
      p_normal.normalize();

      for (Normal3* aux_p : adjacents)
        (*aux_p) = Normal3(p_normal);

    } while (init < local_coordenates_faces.size());
  };

  std::vector<std::future<void>> promises (N_THREADS);
  for (unsigned i = 0; i < N_THREADS; i++)
    promises[i] = std::async(lambda, i);

  for (auto& promise : promises)
    promise.get();

  global_coordenates_faces       = local_coordenates_faces;
  local_coordenates_faces.shrink_to_fit();
  global_coordenates_faces.shrink_to_fit();
}

