#ifndef RASTERISER_H
#define RASTERISER_H

#include "world.h"
#include "spatial/camera.h"
#include "planar/triangle.h"
#include "../auxiliar/multithreadmanager.h"
#include "buffers/commonbuffers.h"

#include <algorithm>

class Projector {
private:    
  std::vector <Mesh*> meshes_vector;

  bool double_faces = false;
  void set_projection_data (Camera& camera);
  void generate_mesh_list (const std::vector<Mesh*>& meshes);  
public:
  Projector();
  void project_camera(Camera& camera);
};

#endif // RASTERISER_H
