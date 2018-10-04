#include "world.h"

World::World(Camera* cm) :
  camera (cm)
  {

  Mesh* a_mesh = new Mesh;
  a_mesh->faces =
    { // face list
      // UPER FACES
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

      Face3({
        {-5, 0, -5},   // vertex a
        { 5, 0, -5},   // vertex b
        { 0,15,  0},   // vertex c
      }),

      Face3({
        { 5, 0, 5},   // vertex a
        {-5, 0, 5},    // vertex b
        { 0,15, 0},    // vertex c
      }),

      // DOWN FACES
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
        {-5,  0, 5},   // vertex a
        {-5,  0,-5},   // vertex b
        { 0,-15, 0},    // vertex c
      }),

      Face3({
        { 5,  0, 5},   // vertex a
        { 5,  0,-5},   // vertex b
        { 0,-15, 0},    // vertex c
      }),
  };

  meshes.push_back(a_mesh);
  a_mesh->color = {0, 200, 200};

  Mesh* aux1 = new Mesh (*a_mesh);
  aux1->translate_local({20, 0, 0});

  Mesh* aux2 = new Mesh (*a_mesh);
  aux2->translate_local({20, 0, 0});

  a_mesh->add_nested_mesh(aux1);
  aux1->add_nested_mesh(aux2);

  for (unsigned i = 0; i < 50; i++) {
      Mesh* aux1 = new Mesh (*a_mesh);
      Mesh* aux2 = new Mesh (*a_mesh);

      aux1->position = {-100, -70, i * 250};
      aux2->position = { 100, -70, i * 250};

      aux1->color = {255, 0, 255};
      aux2->color = {255, 0, 255};

      meshes.push_back(aux1);
      meshes.push_back(aux2);
  }

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

  a_mesh->position = {0, 0, 600};
  b_mesh->position = {0, 0, 100};

  meshes.push_back(b_mesh);
  b_mesh->color = {255, 10, 25};

}

void World::move_right() {
  meshes[0]->rotate_z(0.01);
  meshes[0]->nested_meshes[0]->rotate_x(0.01);
  //meshes[0]->rotate_x(0.01);
  //meshes[0]->position += Point3 {0.0, -0.01, -0.1};


  for (unsigned i = 2; i < meshes.size() - 1; i++) {
    meshes[i]->rotate_y(0.1);
  }

  for (auto& mesh : meshes) {
    //mesh->position += Point3 {0.0, -0.01, -0.1};
    //mesh->rotate_z(0.001);
    //mesh->rotate_y(0.001);
  }

  //camera->position += ;
  //camera->translate_global({0.0, 0.0, 01});
  camera->position += Vector3{0.0, 0.0, 01};
  //camera->rotate_x(0.0010);
  //camera->basis.b += {0, -1, 0};
}

