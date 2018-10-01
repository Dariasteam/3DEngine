#include "world.h"

World::World() {

  Mesh* a_mesh = new Mesh;

  a_mesh->basis = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
  };

  a_mesh->position = {0, 0, 50};

  a_mesh->faces =
    { // face list
      { // one face
        {-5,  0, 0},   // vertex a
        { 5,  0, 0},   // vertex b
        { 0,-10, 5},   // vertex c
      },

      {
        {-5, 0,  10},   // vertex a
        { 5, 0,  10},   // vertex b
        { 0,-10,  5},   // vertex c
      },

      {
        {-5, 0,  0},   // vertex a
        {-5, 0, 10},   // vertex b
        { 0,10,  5},   // vertex c
      },

      {
        { 5, 0,  0},   // vertex a
        { 5, 0, 10},   // vertex b
        { 0,10,  5},   // vertex c
      },

    };

  meshes.push_back(a_mesh);

}

void World::move_right() {
  for (auto& mesh : meshes) {
    mesh->position += {0.01, 0.01, 0};
  }
}

