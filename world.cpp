#include "world.h"

World::World(Camera* cm) :
  camera (cm)
  {

  Mesh* a_mesh = new Mesh;
  a_mesh->faces =
    { // face list
      Face3({ // one face
        {-5,  0, -5},   // vertex a
        { 5,  0, -5},   // vertex b
        { 0,-15, 0},    // vertex c
      }),

      Face3({
        {-5, 0,  5},   // vertex a
        { 5, 0,  5},   // vertex b
        { 0,-15, 0},   // vertex c
      }),

      Face3({
        {-5, 0, -5},   // vertex a
        {-5, 0,  5},   // vertex b
        { 0,15,  0},   // vertex c
      }),

      Face3({
        { 5, 0, -5},   // vertex a
        { 5, 0, 5},    // vertex b
        { 0,15, 0},    // vertex c
      }),

    };

  Mesh* b_mesh = new Mesh;
  b_mesh->faces =
    {
      {
        {-50, -50,  0},
        {-50, -50,10000},
        {+50, -50,  0},
      },
      {
        {+50, -50,  0},
        {+50, -50,10000},
        {-50, -50,10000},
      },
    };

  a_mesh->position = {0, 0, 200};
  b_mesh->position = {0, 0, 100};

  meshes.push_back(a_mesh);
  meshes.push_back(b_mesh);

}

void World::move_right() {
  meshes[0]->rotate_y(0.001);
  for (auto& mesh : meshes) {
    //mesh->position += Point3 {0.0, -0.01, -0.1};
    //mesh->rotate_z(0.001);
    //mesh->rotate_y(0.001);
  }

  camera->position += Point3 {0.0, 0.00, 0.02};
  //camera->rotate_z(0.001);
  //camera->basis.b += {0, -1, 0};
}

