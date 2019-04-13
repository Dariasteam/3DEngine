#include "world.h"

World::World(Camera* cm) :
  camera (cm)
  {

  Mesh* a_mesh = new Mesh;
  a_mesh->local_coordinates_faces =
    { // face list
      // UPER FACES

      Face3({
        {-5, 0, -5},   // vertex a
        { 0,15,  0},   // vertex c
        {-5, 0,  5},   // vertex b
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

      // BOTTOM FACES
      Face3({ // one face
        {-5,  0, -5},   // vertex a
        { 0,-15, 0},    // vertex c
        { 5,  0, -5},   // vertex b
      }),

      Face3({
        {-5, 0,  5},   // vertex a
        { 5, 0,  5},   // vertex b
        { 0,-15, 0},   // vertex c
      }),

      Face3({
        {-5,  0, 5},   // vertex a
        { 0,-15, 0},   // vertex c
        {-5,  0,-5},   // vertex b
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

  a_mesh->generate_nested_normals();

  for (unsigned i = 0; i < 50; i++) {
      Mesh* aux1 = new Mesh (*a_mesh);
      Mesh* aux2 = new Mesh (*a_mesh);

      aux1->position = {-100, -70, 200 + i * 250};
      aux2->position = { 100, -70, 200 + i * 250};

      aux1->color = {255, 0, 255};
      aux2->color = {255, 0, 255};

      meshes.push_back(aux1);
      meshes.push_back(aux2);
  }

  Mesh* b_mesh = new Mesh;
  b_mesh->local_coordinates_faces =
    {
      {
        {-50, -50,  0},
        {+50, -50,  0},
        {-50, -50,10000},
      },
      {
        {+50, -50,  0},
        {+50, -50,10000},
        {-50, -50,10000},
      },
    };

  b_mesh->color = {255, 10, 25};
  b_mesh->generate_normals();
  meshes.push_back(b_mesh);

  a_mesh->position = {0,-30, 400};
  b_mesh->position = {0, 0, 100};
}

void World::move_right() {
  meshes[0]->translate_local({0, 0, 15});
  meshes[0]->rotate_y(0.01);
  meshes[0]->nested_meshes[0]->rotate_x(0.01);
  meshes[0]->rotate_x(0.01);
  meshes[0]->nested_meshes[0]->nested_meshes[0]->rotate_y(0.03);

  for (unsigned i = 1; i < meshes.size() - 1; i++) {
    meshes[i]->rotate_y(0.1);
  }

  camera->translate_local({0.0, 0.0, 15.3});
}

