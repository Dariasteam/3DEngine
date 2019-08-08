#ifndef RASTERISER_H
#define RASTERISER_H

#include "camera.h"
#include "world.h"
#include "canvas.h"
#include "../auxiliar/multithreadmanager.h"

#include <future>
#include <mutex>
#include <fstream>
#include <chrono>
#include <ctime>
#include <algorithm>

class Projector {
private:
  std::mutex mtx;

  World* world;
  Camera* camera;

  bool double_faces = false;
  std::vector<Triangle2i> elements_to_render;

  bool inline calculate_cut_point (const Point3&, const Vector3& dir_v, Point3&) const;
  bool inline is_point_between_camera_bounds (const Point2&) const;

  std::vector <Mesh*> meshes_vector;

  inline Color calculate_lights (const Color& m_color, const Vector3& normal) const;
  void set_projection_data ();
  inline bool calculate_mesh_projection (const Face& face,
                                         std::vector<Triangle2i>& triangles,
                                         const Color& color);

  void generate_mesh_list (const std::vector<Mesh*>& meshes);

  inline void multithreaded_rasterize_mesh_list (unsigned init,
                                                 unsigned end);

  inline void multithreaded_rasterize_single_mesh (unsigned init,
                                                   unsigned end,
                                                   const Mesh* aux_mesh);

  inline bool triangle_inside_screen (const Triangle2i& triangle);
  inline Triangle2i triangle_to_screen_space (const Triangle2& triangle);
public:
  Projector(Camera* camera, World* world);
  std::vector<Triangle2i>& project();


};

#endif // RASTERISER_H
