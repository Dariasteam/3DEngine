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
        {0,  0,  0}    // fake_normal
      }),

      Face3({
        { 5, 0, -5},   // vertex a
        { 5, 0, 5},    // vertex b
        { 0,15, 0},    // vertex c
        {0,  0,  0}    // fake_normal
      }),

      Face3({
        {-5, 0, -5},   // vertex a
        { 5, 0, -5},   // vertex b
        { 0,15,  0},   // vertex c
        {0,  0,  0}    // fake_normal
      }),

      Face3({
        { 5, 0, 5},    // vertex a
        {-5, 0, 5},    // vertex b
        { 0,15, 0},    // vertex c
        {0, 0,  0}     // fake_normal
      }),

      // BOTTOM FACES
      Face3({ // one face
        {-5,  0, -5},   // vertex a
        { 0,-15, 0},    // vertex c
        { 5,  0, -5},   // vertex b
        {0,  0,  0}     // fake_normal
      }),

      Face3({
        {-5, 0,  5},   // vertex a
        { 5, 0,  5},   // vertex b
        { 0,-15, 0},   // vertex c
        {0,  0,  0}    // fake_normal
      }),

      Face3({
        {-5,  0, 5},   // vertex a
        { 0,-15, 0},   // vertex c
        {-5,  0,-5},   // vertex b
        {0,  0,  0}    // fake_normal
      }),

      Face3({
        { 5,  0, 5},   // vertex a
        { 5,  0,-5},   // vertex b
        { 0,-15, 0},   // vertex c
        {0,  0,  0}    // fake_normal
      }),
  };

  add_mesh(a_mesh);
  a_mesh->color = {0, 200, 200};
  a_mesh->generate_data();  

  Mesh* aux1 = new Mesh (*a_mesh);
  aux1->translate_local({20, 0, 0});

  Mesh* aux2 = new Mesh (*a_mesh);
  aux2->translate_local({20, 0, 0});

  a_mesh->add_nested_mesh(aux1);
  aux1->add_nested_mesh(aux2);

  for (unsigned i = 0; i < 30000; i++) {
    Mesh* aux1 = new Mesh (*a_mesh);
    Mesh* aux2 = new Mesh (*a_mesh);

    aux1->position = {-100, -70, static_cast<double>(i * 140)};
    aux2->position = { 100, -70, static_cast<double>(i * 140)};    

    aux1->color = {255, 0, 255};
    aux2->color = {255, 0, 255};

    add_mesh(aux1);
    add_mesh(aux2);
  }

  Mesh* b_mesh = new Mesh;
  b_mesh->local_coordinates_faces =
    {
      {
        {-50, -50,  0},
        {+50, -50,  0},
        {-50, -50,10000},
        {0,  0,  0}    // fake_normal
      },
      {
        {+50, -50,  0},
        {+50, -50,10000},
        {-50, -50,10000},
        {0,  0,  0}    // fake_normal
      },
    };

  add_mesh(b_mesh);
  b_mesh->color = {255, 10, 25};
  b_mesh->generate_data();

  a_mesh->position = {0,-30, 400};
  b_mesh->position = {0, 0, 100};  
}

void World::add_mesh(Mesh* mesh) {
  meshes.push_back(mesh);
}

void World::delete_mesh(Mesh* mesh) {
  for (auto& sub_mesh : mesh->get_nested_meshes())
    delete_mesh(sub_mesh);
  meshes.erase(std::remove(meshes.begin(), meshes.end(), mesh), meshes.end());
}

void World::rotate_meshes() const {
  auto lambda = [&](unsigned start, unsigned end){
    auto it_1 = meshes.begin();
    auto it_2 = meshes.begin();

    std::advance (it_1, start);
    std::advance (it_2, end);

    for (it_1; it_1 < it_2; it_1++)
      (*it_1)->rotate_y(0.1);
  };

  unsigned segments = (meshes.size() / N_THREADS);
  std::vector<std::future<void>> promises (N_THREADS);

  for (unsigned i = 0; i < N_THREADS - 1; i++)
    promises[i] = std::async(lambda, (i * segments) + 3, ((i + 1) * segments) - 3);
  promises[N_THREADS - 1] = std::async(lambda, ((N_THREADS - 1) * segments) - 3, meshes.size() -1);

  for (auto& promise : promises)
    promise.get();
}

void World::calculate_next_frame() const {

  auto* front = meshes.front();

  front->translate_local({0, 0, 15});
  front->rotate_y(0.01);
  front->nested_meshes[0]->rotate_x(0.01);
  front->rotate_x(0.01);
  front->nested_meshes[0]->nested_meshes[0]->rotate_y(0.1);  

  //rotate_meshes();

  camera->translate_local({0.0, 0.0, 15.3});
//  camera->rotate_y(0.01);
}

