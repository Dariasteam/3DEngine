#include "projector.h"

Projector::Projector() {}

void Projector::project_camera(Camera& camera) {
  set_projection_data(camera);
  camera.project(meshes_vector);
}

void Projector::generate_mesh_list(const std::vector<Mesh*> &meshes) {
  for (const auto& mesh : meshes) {
    meshes_vector.push_back(mesh);
    generate_mesh_list(mesh->get_nested_meshes());
  }
}

void Projector::set_projection_data(Camera& camera) {
  const auto& meshes = World::get().get_elements();

  for (unsigned i = 0; i < meshes.size(); i++) {
    meshes[i]->express_in_parents_basis(camera.basis,
                                        camera.position);
  }

  meshes_vector.clear();
  generate_mesh_list(World::get().get_elements());
}
