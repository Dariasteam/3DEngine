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

  meshes.push_back(a_mesh);
/*
  for (unsigned i = 0; i < 10; i++) {
      Mesh* aux1 = new Mesh (*a_mesh);
      Mesh* aux2 = new Mesh (*a_mesh);

      aux1->position = {-100, -50, i * 100};
      aux2->position = { 100, -50, i * 100};

      meshes.push_back(aux1);
      meshes.push_back(aux2);
  }
*/
  Mesh* b_mesh = new Mesh;
  b_mesh->faces =
    {
      {
        {-50, -50,  0},
        {-50, -50,1000},
        {+50, -50,  0},
      },
      {
        {+50, -50,  0},
        {+50, -50,1000},
        {-50, -50,1000},
      },
    };

  a_mesh->position = {0, 0, 200};
  b_mesh->position = {0, 0, 100};

  meshes.push_back(b_mesh);

}

void World::move_right() {
  meshes[0]->rotate_y(0.01);
  meshes[0]->rotate_x(0.01);
  //meshes[0]->position += Point3 {0.0, -0.01, -0.1};


  for (unsigned i = 1; i < meshes.size() - 1; i++) {
    meshes[i]->rotate_y(0.01);
  }
  for (auto& mesh : meshes) {
    //mesh->position += Point3 {0.0, -0.01, -0.1};
    //mesh->rotate_z(0.001);
    //mesh->rotate_y(0.001);
  }

  //camera->position += ;
  //camera->translate_global({0.0, 0.0, 0.1});
  camera->rotate_x(0.0010);
  //camera->basis.b += {0, -1, 0};
}

