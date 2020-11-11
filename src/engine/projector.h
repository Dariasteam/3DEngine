#ifndef RASTERISER_H
#define RASTERISER_H

#include "spatial/camera.h"
#include "world.h"
#include "planar/triangle.h"
#include "../auxiliar/multithreadmanager.h"
#include "rasteriser/abstractrasteriser.h"

#include <future>
#include <mutex>
#include <fstream>
#include <chrono>
#include <ctime>
#include <algorithm>

class Projector {
private:
  CommonBuffers& buffers;
  World* world;
  PerspectiveCamera* camera;

  bool double_faces = false;

  bool inline calculate_cut_point (const Point3&, const Vector3& dir_v, Point2&) ;
  bool inline is_point_between_camera_bounds (const Point2&) const;

  std::vector <Mesh*> meshes_vector;

  void set_projection_data ();
  inline bool calculate_mesh_projection (const Face& face,
                                         const UV& uv,
                                         unsigned thread_index);

  void generate_mesh_list (const std::vector<Mesh*>& meshes);
  bool triangle_inside_camera(const Triangle &triangle);
public:
  Projector(PerspectiveCamera* camera, World* world);
  void project();
};

#endif // RASTERISER_H
