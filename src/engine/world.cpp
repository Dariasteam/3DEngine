#include "world.h"

World::World() :
  principal_camera(Vector3{0, 0, 1},
                   RectF{-1, -1, 1, 1}
                   ),
  sun(Vector3 {0, 0, 1},
      Color {1, 1, 1},
      2)
{
  ObjParser parser;
  Mesh* mesh_1 = parser ("/home/darias/Desarrollo/3D/mesh_examples/file.obj");

  // temporary since we don't have real uvs
  mesh_1->uv_per_face.resize(mesh_1->local_coordenates_faces.size());
  unsigned size = mesh_1->uv_per_face.size();
  for (unsigned i = 0; i < size; i++) {
    mesh_1->uv_per_face[i] = {
      {0, 0},
      {0, .9},
      {.9, .9}
    };
  }

  mesh_1->rotate_z(1);
  mesh_1->set_position_global({0, -2.5, 35});

  Texture <unsigned char, 3> tex;
  tex.load ("/home/darias/Desarrollo/3D/line_texture.ppm");
  mesh_1->texture = tex;
  add_mesh(mesh_1);

  /*

  Mesh* mesh_2 = new Mesh (*mesh_1);
  mesh_2->translate_global({0, -2, 30});
  mesh_2->scale(3);

  add_mesh(mesh_2);
  */

  principal_camera.set_position_global({0, 0, 0});
  //principal_camera.rotate_y(-.5);

  sun.set_position_global({-2, 0, 0});
//  sun.rotate_x(+0.3);
  sun.rotate_y(.5);
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
 meshes.front()->rotate_y(-0.008);
//  meshes.front()->position += Point3{0, 0, 1};
  meshes.front()->rotate_x(-0.003);
  meshes.front()->rotate_z(-0.02);



}

