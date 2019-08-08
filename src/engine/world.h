#ifndef WORLD_H
#define WORLD_H

#include "math/point3.h"
#include "math/vector3.h"
#include "spatial/mesh.h"
#include "camera.h"
#include "../auxiliar/objparser.h"

#include <vector>
#include <algorithm>
#include <future>

struct DirectionalLight {
  Vector3 direction;
  Color color;
  double intensity;
};

class World {
private:
  Camera* camera;
  std::vector <Mesh*> meshes;

  DirectionalLight sunlight {
    Vector3 {1, 0.5, 0},
    Color {255, 255, 255},
    0.5 //1.2
  };

public:
  World (Camera* cm);

  inline const std::vector <Mesh*>& get_elements () { return meshes; }
  inline const DirectionalLight get_light () { return sunlight; }

  inline bool add_mesh (Mesh* mesh);
  inline void delete_mesh (Mesh* mesh);
  inline Camera* get_camera () { return camera; }

  void calculate_next_frame () const;

  ~World () {
    meshes.clear();
  }
};

#endif // WORLD_H
