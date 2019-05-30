#include "point3d.h"

std::list<Mesh*> Mesh::express_in_parents_basis(const Basis3 &new_basis) {
  std::list<Mesh*> mesh_list {this};

  for (auto& nested_mesh : nested_meshes)
    mesh_list.splice(mesh_list.end(), nested_mesh->express_in_parents_basis(new_basis));

  if (base_changed) {
    copy_faces_local2global();
    Matrix3 basis_changer;
    MatrixOps::generate_basis_change_matrix(basis, new_basis, basis_changer);

    for (const auto& mesh : mesh_list) {
      for (auto& face : mesh->global_coordinates_faces) {
        for (unsigned j = 0; j < 3; j++) {
          Point3Ops::change_basis(basis_changer, face[j], face[j]);
          face[j] += position;
        }
        Point3Ops::change_basis(basis_changer, face.normal, face.normal);
      }
    }

  } else if (position_changed) {
    copy_faces_local2global();
    for (const auto& mesh : mesh_list)
      for (auto& face : mesh->global_coordinates_faces)
        for (unsigned j = 0; j < 3; j++)
          face[j] += position;
  }

  position_changed = false;
  base_changed     = false;

  return mesh_list;
}

void Face3::generate_normal() {
  const Vector3& u = Vector3::create_vector(a, b);
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
