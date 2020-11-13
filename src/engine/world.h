#ifndef WORLD_H
#define WORLD_H

#include "math/point3.h"
#include "math/vector3.h"
#include "spatial/mesh.h"
#include "spatial/perspectivecamera.h"
#include "spatial/parallelcamera.h"
#include "spatial/directionallight.h"
#include "../auxiliar/objparser.h"

#include <vector>
#include <algorithm>
#include <future>

class World {
private:
  ParallelCamera principal_camera;
  std::vector <Mesh*> meshes;
  DirectionalLight sun;

  World();
public:
  inline static World& get () {
    static World instance;
    return instance;
  }

  World (MultithreadManager const &) = delete;
  void operator= (World const &) = delete;

  inline const std::vector <Mesh*>& get_elements () { return meshes; }
  inline const DirectionalLight get_light () const { return sun; }

  inline bool add_mesh (Mesh* mesh);
  inline void delete_mesh (Mesh* mesh);
  inline Camera& get_camera () { return principal_camera; }

  void calculate_next_frame ();

  ~World () {
    meshes.clear();
  }
};

#endif // WORLD_H
