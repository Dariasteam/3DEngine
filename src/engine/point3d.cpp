#include "point3d.h"

std::list<Mesh*> Mesh::express_in_parents_basis(const Basis3 &new_basis) {      
  std::list<Mesh*> mesh_list {this};
  bool is_same_base = (basis == new_basis);
  copy_faces_local2global();

  for (auto& nested_mesh : nested_meshes)
    mesh_list.splice(mesh_list.end(), nested_mesh->express_in_parents_basis(new_basis));

  if (!is_same_base) {
    const Matrix3& basis_changer = MatrixOps::generate_basis_change_matrix(basis, new_basis);

    for (const auto& mesh : mesh_list) {
      for (auto& face : mesh->global_coordinates_faces) {
        for (unsigned j = 0; j < 3; j++) {
          Point3Ops::change_basis(basis_changer, face[j], face[j]);
          face[j] += position;
        }
        Point3Ops::change_basis(basis_changer, face.normal, face.normal);
      }
    }

  } else {
    for (const auto& mesh : mesh_list)
      for (auto& face : mesh->global_coordinates_faces)
        for (unsigned j = 0; j < 3; j++)
          face[j] +=  position;
  }

  return mesh_list;
}

void Face3::generate_normal() {
  const Vector3& u = Vector3::create_vector(a, b);
  const Vector3& v = Vector3::create_vector(a, c);

  double X = (v.y() * u.z() - v.z() * u.y());
  double Y = (v.z() * u.x() - v.x() * u.z());
  double Z = (v.x() * u.y() - v.y() * u.x());

  double D = a.z() * v.y() * u.x() +
      a.x() * v.z() * u.y() +
      a.y() * v.x() * u.z() -
      a.x() * v.y() * u.z() -
      a.y() * v.z() * u.x() -
      a.z() * v.x() * u.y();

  normal = {X, Y, Z};
}


void Point3Ops::change_basis(const Basis3 &basis,
                             const Point3 &element,
                                   Point3 &result) {
  double a = (basis[0][0] * element[0] + basis[0][1] * element[1] + basis[0][2] * element[2]);
  double b = (basis[1][0] * element[0] + basis[1][1] * element[1] + basis[1][2] * element[2]);
  double c = (basis[2][0] * element[0] + basis[2][1] * element[1] + basis[2][2] * element[2]);

  result[0] = a;
  result[1] = b;
  result[2] = c;
}
