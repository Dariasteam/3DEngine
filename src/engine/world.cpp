#include "world.h"

World::World(Camera* cm) :
  camera (cm)
  {
/*
  Mesh* a_mesh = new Mesh;
  a_mesh->local_coordenates_faces = { // face list
    // UPER FACES
    Face3({          // RIGHT FACE
      {-5, 0, -5},   // vertex a
      {-5, 0,  5},   // vertex c
      { 0,15,  0},   // vertex b
      {0,  0,  0}    // fake_normal
    }),


    Face3({          // LEFT FACE
      { 5, 0, -5},   // vertex a
      { 0,15,  0},   // vertex c
      { 5, 0,  5},   // vertex b
      {0,  0,  0}    // fake_normal
    }),


    Face3({          // FRONT FACE
      { 5, 0, -5},   // vertex b
      {-5, 0, -5},   // vertex a
      { 0,15,  0},   // vertex c
      {0,  0,  0}    // fake_normal
    }),

    Face3({          // BACK FACE
      { 5, 0, 5},    // vertex a
      { 0, 15, 0},   // vertex c
      {-5, 0, 5},    // vertex b
      {0, 0,  0}     // fake_normal
    }),

    // BOTTOM FACES
    Face3({          // RIGHT FACE
      {-5, 0, -5},   // vertex a
      { 0,-15,  0},  // vertex b
      {-5, 0,  5},   // vertex c
      {0,  0,  0}    // fake_normal
    }),

    Face3({          // LEFT FACE
      { 5, 0, -5},   // vertex a
      { 5, 0,  5},   // vertex b
      { 0,-15,  0},  // vertex c
      {0,  0,  0}    // fake_normal
    }),

    Face3({          // FRONT FACE
      {-5, 0, -5},   // vertex a
      { 5, 0, -5},   // vertex b
      { 0,-15,  0},  // vertex c
      {0,  0,  0}    // fake_normal
    }),

    Face3({          // BACK FACE
      { 5, 0, 5},    // vertex a
      {-5, 0, 5},    // vertex b
      { 0,-15, 0},   // vertex c
      {0, 0,  0}     // fake_normal
    }),
  };
*/
//  add_mesh(a_mesh);
//  a_mesh->color = {0, 200, 200};
//  a_mesh->generate_data();
//  a_mesh->translate_global({100, -80, 200});
/*
  Mesh* aux1 = new Mesh (*a_mesh);
  aux1->translate_global({20, 0, 0});

  Mesh* aux2 = new Mesh (*a_mesh);
  aux2->translate_global({20, 0, 0});
  */
/*
  a_mesh->add_nested_mesh(aux1);
  aux1->add_nested_mesh(aux2);
*/
  //125000
/*
  for (unsigned i = 0; i < 1200; i++) {
    Mesh* aux1 = new Mesh (*a_mesh);
    Mesh* aux2 = new Mesh (*a_mesh);

    aux1->translate_global ({-100, 0, static_cast<double>(i * 100)});
    aux2->translate_global ({ 100, 0, static_cast<double>(i * 100)});

    aux1->color = {255, 0, 255};
    aux2->color = {255, 0, 255};

//    add_mesh(aux1);
//    add_mesh(aux2);
  }
*/
/*
  Mesh* b_mesh = new Mesh;
  b_mesh->local_coordenates_faces =
    {
      {
        {-50, -50,  0},
        {-50, -50,10000},
        {+50, -50,  0},
        {0,  0,  0}    // fake_normal
      },
      {
        {+50, -50,  0},
        {-50, -50,10000},
        {+50, -50,10000},
        {0,  0,  0}    // fake_normal
      },
    };
*/
//  add_mesh(b_mesh);
//  b_mesh->color = {255, 10, 25};
//  b_mesh->generate_data();

//  b_mesh->position = {0, 0, 100};
//  a_mesh->position = {0, 10, 50};

  ObjParser parser;
  Mesh* parsed_mesh = parser ("../../mesh_examples/file.obj");
  add_mesh(parsed_mesh);
  parsed_mesh->color = {250, 150, 100};
  parsed_mesh->rotate_y(10);
  parsed_mesh->translate_global({0, -1.5, 50});
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

void World::calculate_next_frame() const {

  //auto* front = meshes.front();
  //front->translate_local({0, 0, 2});
/*
  front->rotate_y(0.005);
  front->translate_local({0, 0, 13.2});
  front->nested_meshes[0]->rotate_x(0.01);
  front->rotate_x(0.01);
  front->nested_meshes[0]->nested_meshes[0]->rotate_y(0.1);
*/
/*
  for (auto& mesh : meshes)
    mesh->rotate_y(-0.03);
*/
  meshes.back()->rotate_y(-0.003);

//  meshes.back()->color += {1, -1, 1};
//  meshes.back()->rotate_x(-0.03);
//  meshes.back()->rotate_z(-0.01);

//  meshes.back()->translate_global({0, 0, 0.09});

  //rotate_meshes();

// camera->translate_global({0, 0.0, 1});
//   camera->rotate_z(-0.1);
}

