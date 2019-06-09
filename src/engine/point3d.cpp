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
                                      const Point3 &translation,
                                      bool update_rotation,
                                      bool update_translation) {


  Matrix3 basis_changer_1;
  MatrixOps::generate_basis_change_matrix(basis, new_basis, basis_changer_1);

  Matrix3 basis_changer_2;
  MatrixOps::generate_basis_change_matrix(canonical_base, new_basis, basis_changer_2);

  Vector3 aux_pos;
  Point3Ops::change_basis(basis_changer_2, translation, aux_pos);

  aux_pos += translation;

  global_coordenates_faces = local_coordenates_faces;

  for (const auto& mesh : mesh_list) {
    auto lambda = [&](unsigned from, unsigned to) {
      mesh->change_basis_part(basis_changer_1, aux_pos, from, to);
    };

    unsigned size = mesh->global_coordenates_faces.size();
    unsigned segment = size / N_THREADS;
    std::vector<std::future<void>> promises (N_THREADS);
    for (unsigned i = 0; i < N_THREADS - 1; i++)
      promises[i] = std::async(lambda, i * segment, (i + 1) * segment);
    promises[N_THREADS - 1] = std::async(lambda, (N_THREADS - 1) * segment, size);

    for (auto& promise : promises)
      promise.get();
  }
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

//  if (update_translation || update_rotation) {/
    Matrix3 basis_changer_2;
    MatrixOps::generate_basis_change_matrix(canonical_base, new_basis, basis_changer_2);

    if (basis_changed || camera_rotated)
      Point3Ops::change_basis(basis_changer_2, position, aux_pos);
    else if (position_changed)
      Point3Ops::change_basis(basis_changer_2, translation, aux_pos);

    aux_pos -= camera_translation;
//  }

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

  change_basis (mesh_list, new_basis, camera_translation, camera_rotated,
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
