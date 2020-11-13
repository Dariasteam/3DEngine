#include "world.h"

World::World() :
  principal_camera(Vector3{0, 0, 1},
                   Point3{0, 0, 3}
                   ),
  sun(Vector3 {.5, .5, 0},
      Color {1, 1, 1},
      3.0)
{
  ObjParser parser;
  Mesh* parsed_mesh = parser ("/home/darias/Desarrollo/3D/mesh_examples/default_cube.obj");

  parsed_mesh->rotate_y(10);
  parsed_mesh->translate_global({0, 0, 13});

  // temporary since we don't have real uvs
  parsed_mesh->uv_per_face.resize(parsed_mesh->local_coordenates_faces.size());
  unsigned size = parsed_mesh->uv_per_face.size();
  for (unsigned i = 0; i < size; i++) {

    parsed_mesh->uv_per_face[i] = {
      {0, 0},
      {0, .9},
      {.9, .9}
    };
  }

  Texture <unsigned char, 3> tex;
  tex.load ("/home/darias/Desarrollo/3D/line_texture.ppm");
  parsed_mesh->texture = tex;

  add_mesh(parsed_mesh);
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

bool movingLeft = false;

void World::calculate_next_frame() const {
  meshes.front()->rotate_y(-0.008);
  meshes.front()->rotate_x(-0.003);
  meshes.front()->rotate_z(-0.02);
}

