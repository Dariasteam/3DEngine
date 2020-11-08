#include "world.h"

World::World(Camera* cm) :
  camera (cm)
  {

  ObjParser parser;
  Mesh* parsed_mesh = parser ("/home/darias/Desarrollo/3D/mesh_examples/default_cube.obj");

  parsed_mesh->color = {250, 150, 100};
  parsed_mesh->rotate_y(10);
  parsed_mesh->translate_global({0, 0, 30});

  // temporary since we don't have real uvs
  parsed_mesh->uv_per_face.resize(parsed_mesh->local_coordenates_faces.size());
  unsigned size = parsed_mesh->uv_per_face.size();
  for (unsigned i = 0; i < size; i++) {

    parsed_mesh->uv_per_face[i] = {
      {0, 0},
      {0, .5},
      {.5, .5}
    };
  }

  Texture <unsigned char, 3> tex;
  tex.load ("line_texture.ppm");
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
  meshes.front()->rotate_x(-0.013);
  meshes.front()->rotate_z(-0.02);
}

