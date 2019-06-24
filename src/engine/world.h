#ifndef WORLD_H
#define WORLD_H

#include "point3d.h"
#include "camera.h"
#include "objparser.h"

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

  const Basis3 basis {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
  };  

  inline const std::vector <Mesh*>& get_elements () { return meshes; }
  inline const DirectionalLight get_light () { return sunlight; }

  inline bool add_mesh (Mesh* mesh);
  inline void delete_mesh (Mesh* mesh);

  inline void rotate_meshes () const;  

  void calculate_next_frame () const;

  ~World () {
    meshes.clear();
  }
};

#endif // WORLD_H
