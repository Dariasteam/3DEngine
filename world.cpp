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

  a_mesh->position = {0, 0, 450};

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



  Mesh* b_mesh = new Mesh;

  b_mesh->basis = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
  };

  b_mesh->position = {0, 0, 220};

  b_mesh->faces =
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


  meshes.push_back(a_mesh);
  meshes.push_back(b_mesh);

}

void World::move_right() {
  for (auto& mesh : meshes) {
    //mesh->position += {0.01, 0, 0};
    mesh->rotate_x(0.001);
    mesh->rotate_y(0.001);

    //camera->position += {0.000, 0.001, 0.00};
    camera->rotate_y(0.00001);
    //mesh->basis.a += {-0.0001, 0, 0};
  }
}

