#include "point3d.h"

#include <future>
#include <functional>

// Multithreaded apply rotations to all vertex
void Mesh::apply_rotations (const std::list<Mesh*> mesh_list) {
  intermediate_coordenates_faces = local_coordenates_faces;

  Matrix3 basis_changer_1;
  MatrixOps::generate_basis_change_matrix(basis, canonical_base, basis_changer_1);

  for (const auto& mesh : mesh_list) {
    auto lambda = [&](unsigned from, unsigned to) {
      for (unsigned k = from; k < to; k++) {
        auto& face = mesh->intermediate_coordenates_faces[k];
        for (unsigned j = 0; j < 3; j++)
          Point3Ops::change_basis(basis_changer_1, face[j], face[j]);
      }
    };

    unsigned N_THREADS = 8;
    unsigned size = mesh->intermediate_coordenates_faces.size();
    unsigned segment = size / N_THREADS;
    std::vector<std::future<void>> promises (N_THREADS);
    for (unsigned i = 0; i < N_THREADS - 1; i++)
      promises[i] = std::async(lambda, i * segment, (i + 1) * segment);
    promises[N_THREADS - 1] = std::async(lambda, (N_THREADS - 2) * segment, size);

    for (auto& promise : promises)
      promise.get();
  }
}

// Multithreaded change basis
void Mesh::change_basis(const std::list<Mesh *> mesh_list,
                        const Basis3 &new_basis,
                        const Point3 &translation){

  Matrix3 basis_changer_3;
  MatrixOps::generate_basis_change_matrix(basis, new_basis, basis_changer_3);

  Matrix3 basis_changer_2;
  MatrixOps::generate_basis_change_matrix(canonical_base, new_basis, basis_changer_2);

  global_coordenates_faces = intermediate_coordenates_faces;

  for (const auto& mesh : mesh_list) {
    auto lambda = [&](unsigned from, unsigned to) {
      for (unsigned k = from; k < to; k++) {
        auto& face = mesh->global_coordenates_faces[k];
        for (unsigned j = 0; j < 3; j++) {
          face[j] += position;
          Point3Ops::change_basis(basis_changer_2, face[j], face[j]);
          face[j] += translation;
        }
        Point3Ops::change_basis(basis_changer_3, face.normal, face.normal);
      }
    };

    unsigned N_THREADS = 8;
    unsigned size = mesh->intermediate_coordenates_faces.size();
    unsigned segment = size / N_THREADS;
    std::vector<std::future<void>> promises (N_THREADS);
    for (unsigned i = 0; i < N_THREADS - 1; i++)
      promises[i] = std::async(lambda, i * segment, (i + 1) * segment);
    promises[N_THREADS - 1] = std::async(lambda, (N_THREADS - 2) * segment, size);

    for (auto& promise : promises)
      promise.get();
  }
}

std::list<Mesh*> Mesh::express_in_parents_basis(const Basis3& new_basis,
                                                const Point3& translation) {

  // Generates a list with this mesh and it's nested ones
  std::list<Mesh*> mesh_list {this};

  for (auto& nested_mesh : nested_meshes)
    mesh_list.splice(mesh_list.end(),
                     nested_mesh->express_in_parents_basis(new_basis, translation));

  if (basis_changed)
    apply_rotations(mesh_list);

  change_basis(mesh_list, new_basis, translation);

  basis_changed = false;
  position_changed = false;

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
