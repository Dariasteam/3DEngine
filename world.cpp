#include "world.h"

World::World() {
  /*
  elements.push_back({0,  0, 20});
  elements.push_back({0,  1, 21});
  elements.push_back({0,  2, 22});
  elements.push_back({0,  3, 23});
  elements.push_back({0,  4, 24});

  elements.push_back({0,  3, 25});
  elements.push_back({0,  2, 26});
  elements.push_back({0,  1, 27});
  elements.push_back({0,  0, 28});

  elements.push_back({0,  -1, 27});
  elements.push_back({0,  -2, 26});
  elements.push_back({0,  -3, 25});
  elements.push_back({0,  -4, 24});

  elements.push_back({0,  -3, 23});
  elements.push_back({0,  -2, 22});
  elements.push_back({0,  -1, 21});
  */

  Mesh* a = new Mesh {
    { // face list
      { // one face
        {0, 0, 20},   // vertex a
        {0, 10, 25},  // vertex b
        {0, -10, 20}, // vertex c
      }
    }
  };

  meshes.push_back(a);

}

void World::move_right() {
  for (auto& mesh : meshes) {
    for (auto& face : mesh->faces) {
      face.a.x += 0.01;
      face.b.x += 0.01;
      face.c.x += 0.01;
    }
  }
}
