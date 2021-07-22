#include "mesh.h"

void Mesh::change_basis_part (const Matrix3& basis_changer,
                              const Vector3& translation,
                              unsigned from, unsigned to) {
  for (unsigned k = from; k < to; k++) {
    auto& vertex = vertices[k];
    Point3Ops::change_basis(basis_changer, vertex.point_local, vertex.point_global);
    Point3Ops::change_basis(basis_changer, vertex.normal_local, vertex.normal_global);
    vertex.point_global += translation;
  }
}

// Multithreaded change basis
void Mesh::change_basis_multithreaded(const std::list<Mesh*>& mesh_list,
                                      const Basis3& new_basis,
                                      const Point3& pos) {

  Matrix3 camera_basis_changer;
  MatrixOps::generate_basis_change_matrix(basis, new_basis, camera_basis_changer);

  Vector3 aux_pos;
  Point3Ops::change_basis(new_basis, Vector3(position - pos), aux_pos);

  for (const auto& mesh : mesh_list) {
    unsigned size = mesh->vertices.size();
    double segment = double(size) / N_THREADS;

    for (Face& face : faces)
      Point3Ops::change_basis(camera_basis_changer, face.normal_local, face.normal_global);

    auto& m = MultithreadManager::get_instance();
    m.calculate_threaded(N_THREADS, [&](unsigned i) {
      unsigned from = i * segment;
      unsigned to = (i + 1) * segment;

      mesh->change_basis_part(camera_basis_changer, aux_pos, from, to);
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

void Mesh::generate_data() {
  generate_face_normals();

  for (Vertex& vertex : vertices) {
    std::list<const Face*> contiguous_faces;

    // Search for a contiguous faces
    for (const Face& face : faces) {
      if (face.a == &vertex || face.b == &vertex || face.c == &vertex) {
        contiguous_faces.push_back(&face);
      }
    }

    // Calculate normal
    Vector3 normal {0, 0, 0};

    for (const Face* face : contiguous_faces)
      normal += face->normal_local.toVector3();

    normal.normalize();

    vertex.normal_local = Normal3(normal);
  }
}

