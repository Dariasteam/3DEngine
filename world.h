#ifndef WORLD_H
#define WORLD_H

#include "point3d.h"
#include "camera.h"

#include <vector>

class World {
private:
  Camera* camera;
  std::vector <Mesh*> meshes;
public:
  World(Camera* camera);
  inline const std::vector <Mesh*>& get_elements () { return meshes; }
  void move_right ();
};

#endif // WORLD_H
