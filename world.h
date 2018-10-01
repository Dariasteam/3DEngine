#ifndef WORLD_H
#define WORLD_H

#include "point3d.h"

#include <vector>

class World {
private:
  std::vector <Mesh*> meshes;
public:
  World();
  inline const std::vector <Mesh*>& get_elements () { return meshes; }
  void move_right ();
};

#endif // WORLD_H
