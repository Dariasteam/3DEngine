#include "world.h"

World::World() {
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


}

void World::move_right() {
  for (auto& element : elements) {
    element.x += 0.01;
  }
}
