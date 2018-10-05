#ifndef WORLD_H
#define WORLD_H

#include "point3d.h"
#include "camera.h"

#include <vector>

struct DirectionalLight {
  Vector3 direction;
  Color color;
  unsigned intensity;
};

class World {
private:
  Camera* camera;
  std::vector <Mesh*> meshes;

  DirectionalLight sunlight {
    Vector3 {1, 1, 1},
    Color {255, 255, 255},
    100
  };


public:

  const Basis3 basis {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
  };

  World(Camera* camera);
  inline const std::vector <Mesh*>& get_elements () { return meshes; }
  inline const DirectionalLight get_light () { return sunlight; }
  void move_right ();
};

#endif // WORLD_H
