#include "world.h"

World::World() :
  principal_camera(Vector3{0, 0, 1},
                   RectF{-1, -1, 1, 1}
                   ),
  sun(Vector3 {0, 0, 1},
      Color {1, 1, 1},
      3)
{
  ObjParser parser;
  Mesh* mesh_1 = parser ("/home/darias/Desarrollo/3D/mesh_examples/file.obj");
  mesh_1->uv_per_face.resize(mesh_1->faces.size());
  unsigned size = mesh_1->uv_per_face.size();
  for (unsigned i = 0; i < size; i++) {
    mesh_1->uv_per_face[i] = {
      {0, 0},
      {0, .9},
      {.9, .9}
    };
  }

//  mesh_1->rotate_z(1);
  mesh_1->set_position_global({0, -4, 35});
  add_mesh(mesh_1);

  principal_camera.set_position_global({-8.8, -2, 27});
  principal_camera.rotate_y(-0.8);

  sun.set_position_global({40, 10, 0});
  sun.rotate_y(.8);
  sun.rotate_x(-.2);

  sun.express_in_parent_basis(get_camera().basis);
}


bool World::add_mesh(Mesh* mesh) {
  if (mesh == nullptr)
    return false;
  meshes.push_back(mesh);
  return true;
}

void World::delete_mesh(Mesh* mesh) {
  for (auto& sub_mesh : mesh->get_nested_meshes())
    delete_mesh(sub_mesh);
  meshes.erase(std::remove(meshes.begin(), meshes.end(), mesh), meshes.end());
}

void World::calculate_next_frame() {
  //meshes.front()->rotate_y(0.002);
  //meshes.front()->rotate_x(-0.001);
}

