#include "mesh.h"

// Multithreaded change basis
void Mesh::change_basis_multithreaded(const std::list<Mesh*>& mesh_list,
                                      const Basis3& new_basis,
                                      const Point3& pos) {

  Matrix3 camera_basis_changer;
  MatrixOps::generate_basis_change_matrix(basis, new_basis, camera_basis_changer);

  Vector3 aux_pos;
  Point3Ops::change_basis(new_basis, Vector3(position - pos), aux_pos);

  for (const auto& mesh : mesh_list) {

    auto& m = MultithreadManager::get_instance();

    std::function<void(Face& f)> face_basis_changer = [&](Face& face) {
      Point3Ops::change_basis(camera_basis_changer, face.normal_local, face.normal_global);
    };

    std::function<void(Vertex& f)> vertex_basis_changer = [&](Vertex& vertex) {
      Point3Ops::change_basis(camera_basis_changer, vertex.point_local, vertex.point_global);
      Point3Ops::change_basis(camera_basis_changer, vertex.normal_local, vertex.normal_global);
      vertex.point_global += aux_pos;
    };

    m.calculate_threaded(mesh->faces, face_basis_changer);
    m.calculate_threaded(mesh->vertices, vertex_basis_changer);
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
