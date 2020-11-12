#ifndef RASTERISER_H
#define RASTERISER_H

#include "world.h"
#include "planar/triangle.h"
#include "../auxiliar/multithreadmanager.h"
#include "buffers/commonbuffers.h"

#include <algorithm>

class Projector {
private:  
  World* world;  
  std::vector <Mesh*> meshes_vector;

  bool double_faces = false;
  void set_projection_data (Camera* camera);
  void generate_mesh_list (const std::vector<Mesh*>& meshes);  
public:
  Projector(World* world);
  void project();
};

#endif // RASTERISER_H
