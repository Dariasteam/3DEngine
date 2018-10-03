#include "world.h"

World::World(Camera* cm) :
  camera (cm)
  {

  Mesh* a_mesh = new Mesh;

  a_mesh->basis = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
  };  

  a_mesh->faces =
    { // face list
      { // one face
        {-5,  0, -5},   // vertex a
        { 5,  0, -5},   // vertex b
        { 0,-15, 0},   // vertex c
      },

      {
        {-5, 0,  5},   // vertex a
        { 5, 0,  5},   // vertex b
        { 0,-15, 0},   // vertex c
      },

      {
        {-5, 0, -5},   // vertex a
        {-5, 0,  5},   // vertex b
        { 0,15,  0},   // vertex c
      },

      {
        { 5, 0, -5},   // vertex a
        { 5, 0, 5},   // vertex b
        { 0,15, 0},   // vertex c
      },

    };


  a_mesh->position = {0, 0, 100};

  meshes.push_back(a_mesh);

}

void World::move_right() {
  for (auto& mesh : meshes) {
    //mesh->position += {0.0, -0.01, 0};
    //mesh->rotate_x(0.001);
    mesh->rotate_y(-0.001);
  }

  //camera->position += {0.0, 0.01, 0.01};
  camera->rotate_y(-0.001);
  //mesh->basis.a += {-0.0001, 0, 0};
}

