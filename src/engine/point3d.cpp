#include "point3d.h"

#include <future>
#include <functional>


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

void Mesh::apply_translation_part (const Vector3& translation,
                                   unsigned from, unsigned to) {
  for (unsigned k = from; k < to; k++) {
    auto& face = global_coordenates_faces[k];
    for (unsigned j = 0; j < 3; j++) {
      face[j] += translation;
    }
  }
}

// Multithreaded change basis
void Mesh::change_basis_multithreaded(const std::list<Mesh*> mesh_list,
                                      const Basis3 &new_basis,
                                      const Point3 &camera_translation,
                                      bool camera_rotated,
                                      bool camera_translated) {


  bool update_rotation    = basis_changed    | camera_rotated;
  bool update_translation = position_changed | camera_translated;

  Matrix3 basis_changer_1;
  MatrixOps::generate_basis_change_matrix(basis, new_basis, basis_changer_1);

  Vector3 aux_pos {0, 0, 0};
  if (update_rotation || update_translation) {
    Matrix3 basis_changer_2;
    MatrixOps::generate_basis_change_matrix(canonical_base, new_basis, basis_changer_2);

    if (update_rotation)
      Point3Ops::change_basis(basis_changer_2, position, aux_pos);
    else if (position_changed)
      Point3Ops::change_basis(basis_changer_2, translation, aux_pos);

    aux_pos -= camera_translation;
  } else {
    return;
  }

  std::function<void (Mesh* mesh, unsigned from, unsigned to)> lambda;

  if (update_rotation) {
    global_coordenates_faces = local_coordenates_faces;
    lambda = [&](Mesh* mesh, unsigned from, unsigned to) {
      mesh->change_basis_part(basis_changer_1, aux_pos, from, to);
    };
  } else {
    lambda = [&](Mesh* mesh, unsigned from, unsigned to) {
      mesh->apply_translation_part(aux_pos, from, to);
    };
  }

  for (const auto& mesh : mesh_list) {
    unsigned size = mesh->global_coordenates_faces.size();
    unsigned segment = size / N_THREADS;
    std::vector<std::future<void>> promises (N_THREADS);
    for (unsigned i = 0; i < N_THREADS - 1; i++)
      promises[i] = std::async(lambda, mesh, i * segment, (i + 1) * segment);
    promises[N_THREADS - 1] = std::async(lambda, mesh, (N_THREADS - 1) * segment, size);

    for (auto& promise : promises)
      promise.get();
  }

  basis_changed = false;
  position_changed = false;
}

void Mesh::change_basis(const std::list<Mesh *> mesh_list,
                        const Basis3 &new_basis,
                        const Point3 &camera_translation,
                        bool camera_rotated,
                        bool camera_translated) {

  bool update_rotation    = basis_changed    | camera_rotated;
  bool update_translation = position_changed | camera_translated;

  Matrix3 basis_changer_1;
  MatrixOps::generate_basis_change_matrix(basis, new_basis, basis_changer_1);

  Vector3 aux_pos {0, 0, 0};
  if (update_rotation || update_translation) {
    Matrix3 basis_changer_2;
    MatrixOps::generate_basis_change_matrix(canonical_base, new_basis, basis_changer_2);

    if (update_rotation)
      Point3Ops::change_basis(basis_changer_2, position, aux_pos);
    else if (position_changed)
      Point3Ops::change_basis(basis_changer_2, translation, aux_pos);

    aux_pos -= camera_translation;
  } else {
    return;
  }

  if (update_rotation) {
    global_coordenates_faces = local_coordenates_faces;

    for (const auto& mesh : mesh_list) {
      mesh->change_basis_part(basis_changer_1, aux_pos, 0,
                              mesh->local_coordenates_faces.size());      
    }
  } else if (update_translation) {      
    for (const auto& mesh : mesh_list) {
      mesh->apply_translation_part(aux_pos, 0,
                                   mesh->local_coordenates_faces.size());
    }        
  }

  basis_changed = false;
  position_changed = false;
}

std::vector<Vector3*> Mesh::get_adjacent_vertices(Point3& p, unsigned from, std::vector<bool>& vertex_normals) {
  std::vector<Vector3*> adjacents;

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

  auto lambda = [&](unsigned i_thread) {
    unsigned init = last_index;
    unsigned j = 0;
    unsigned k = 0;
    bool end = false;

    // find next point not used by another thread
    do {
      mtx.lock();
      init = last_index;
      do {
        for (j = 0; j < 3; j++) {
          for (k = 0; k < current_point_per_thread.size(); k++) {
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

      Point3 point = local_coordenates_faces[init][j];
      Vector3& p_normal = local_coordenates_faces[init].get_normal(j);
      auto adjacents = get_adjacent_vertices(point, ++init, vertex_normals);

      Vector3 ac = p_normal;
      for (Vector3* aux_p : adjacents)
        ac += *aux_p;

      ac /= (adjacents.size() + 1);

      for (Vector3* aux_p : adjacents)
        (*aux_p) = ac;

      p_normal = ac;
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

std::list<Mesh*> Mesh::express_in_parents_basis(const Basis3& new_basis,
                                                const Point3& camera_translation,
                                                bool camera_rotated,
                                                bool camera_translated) {

  // Generates a list with this mesh and it's nested ones
  std::list<Mesh*> mesh_list {this};

  for (auto& nested_mesh : nested_meshes)
    mesh_list.splice(mesh_list.end(),
                     nested_mesh->express_in_parents_basis(new_basis,
                                                           camera_translation, camera_rotated, camera_translated));

  change_basis_multithreaded (mesh_list, new_basis, camera_translation, camera_rotated,
                              camera_translated);

  return mesh_list;
}

void Face3::generate_normal() {
  const Vector3& u = Vector3::create_vector(b, a);
  const Vector3& v = Vector3::create_vector(a, c);

  double X = (v.y() * u.z() - v.z() * u.y());
  double Y = (v.z() * u.x() - v.x() * u.z());
  double Z = (v.x() * u.y() - v.y() * u.x());
  /*
  double D = a.z() * v.y() * u.x() +
             a.x() * v.z() * u.y() +
             a.y() * v.x() * u.z() -
             a.x() * v.y() * u.z() -
             a.y() * v.z() * u.x() -
             a.z() * v.x() * u.y();
*/

  normal = {X, Y, Z};
  normal.normalize();

  normal_a = normal;
  normal_b = normal;
  normal_c = normal;
}


void Point3Ops::change_basis(const Basis3 &basis,
                             const Point3 &element,
                             Point3 &result) {
  double a = (basis[0][0] * element.x() + basis[0][1] * element.y() + basis[0][2] * element.z());
  double b = (basis[1][0] * element.x() + basis[1][1] * element.y() + basis[1][2] * element.z());
  double c = (basis[2][0] * element.x() + basis[2][1] * element.y() + basis[2][2] * element.z());

  result.set_x(a);
  result.set_y(b);
  result.set_z(c);
}
