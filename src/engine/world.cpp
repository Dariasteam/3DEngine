#include "world.h"

World::World(Camera* cm) :
  camera (cm)
  {
/*
  Mesh* a_mesh = new Mesh;
  a_mesh->local_coordenates_faces = { // face list
    // UPER FACES
    Face({          // RIGHT FACE
      {0 , 0,  0},   // vertex a
      {0 ,500,  0},   // vertex c
      {500,500,  0},   // vertex b
      {0 , 0, 1},    // fake_normal

      {0,  0,  1},    // fake_normal_a
      {0,  0,  1},    // fake_normal_b
      {0,  0,  1},    // fake_normal_c
    }),
  };

  Texture tex;
  tex.load ("pic.ppm");
  a_mesh->texture = tex;

  a_mesh->color = {255, 0, 255};
  a_mesh->uv_per_face = {
    {
      {0, 0},
      {0, 1},
      {1, 1}
    }
  };

  a_mesh->generate_data();

  a_mesh->color = {250, 150, 100};
  a_mesh->rotate_y(10);
  a_mesh->translate_global({0, 0, 5});
  add_mesh(a_mesh);
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
  Mesh* parsed_mesh = parser ("/home/darias/Desarrollo/3D/mesh_examples/default_cube.obj");

  //Mesh* mesh_2 = new Mesh(*parsed_mesh);
  add_mesh(parsed_mesh);
  //add_mesh(mesh_2);

  parsed_mesh->color = {250, 150, 100};
  parsed_mesh->rotate_y(10);
  parsed_mesh->translate_global({0, -1.5, 20});

  // temporary since we don't have real uvs
  parsed_mesh->uv_per_face.resize(parsed_mesh->local_coordenates_faces.size());
  for (unsigned i = 0; i < parsed_mesh->uv_per_face.size(); i++) {
    parsed_mesh->uv_per_face[i] = {
      {0, 0},
      {0, 1},
      {1, 1}
    };
  }

  Texture tex;
  tex.load ("pic.ppm");
  parsed_mesh->texture = tex;
/*
  mesh_2->color = {0, 150, 100};
  mesh_2->rotate_y(10);
  mesh_2->translate_global({10, -1.5, 100});
  */
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
  meshes.front()->rotate_y(-0.008);
  meshes.front()->rotate_x(-0.002);
  //meshes.front()->rotate_x(-0.02);
  //meshes.front()->rotate_z(+0.02);

  //auto currentPos = meshes.front()->translation;
  //Vector3 newPos (currentPos.x(), currentPos.y(), currentPos.z());
/*
  if (movingLeft) {
    newPos.set_x(newPos.x() - 0.05);
    if (meshes.front()->translation.x() < -10)
      movingLeft = false;
  } else {
    newPos.set_x(newPos.x() + 0.05);
    if (meshes.front()->translation.x() > 10)
      movingLeft = true;
  }

  meshes.front()->translate_global(newPos);  
*/
//  meshes.back()->color += {1, -1, 1};
//  meshes.back()->rotate_x(-0.03);
//  meshes.back()->rotate_z(-0.01);

//  meshes.back()->translate_global({0, 0, 0.09});

  //rotate_meshes();

//   camera->rotate_z(-0.1);
}

