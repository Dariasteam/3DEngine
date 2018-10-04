#include "world.h"

World::World(Camera* cm) :
  camera (cm)
  {

  Mesh* a_mesh = new Mesh;

  a_mesh->basis = Basis3({
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
  });

  a_mesh->faces =
    { // face list
      Face3({ // one face
        {-5,  0, -5},   // vertex a
        { 5,  0, -5},   // vertex b
        { 0,-15, 0},   // vertex c
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
        { 5, 0, 5},   // vertex b
        { 0,15, 0},   // vertex c
      }),

    };


  a_mesh->position = {0, 0, 200};

  meshes.push_back(a_mesh);

}

void World::move_right() {
  for (auto& mesh : meshes) {
    //mesh->position += Point3 {0.0, -0.01, -0.1};
    //mesh->rotate_z(0.001);
    //mesh->rotate_y(0.001);
  }

  camera->position += Point3 {0.0, 0.01, 0.1};
  //camera->rotate_z(0.0008);
  //camera->basis.b += {0, -1, 0};
}

