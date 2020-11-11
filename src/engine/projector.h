#ifndef RASTERISER_H
#define RASTERISER_H

#include "spatial/camera.h"
#include "world.h"
#include "planar/triangle.h"
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

  std::mutex mtx2;
  std::condition_variable cv2;
  bool cv_bool = false;

  unsigned c {0};


  World* world;
  PerspectiveCamera* camera;

  bool double_faces = false;

  std::vector<Triangle2i> elements_to_render;
  unsigned n_elements_to_render;

  Triangle2i* tmp_triangles_i[N_THREADS];
  Triangle2* tmp_triangles_f[N_THREADS];
  unsigned tmp_triangles_sizes[N_THREADS];

  bool inline calculate_cut_point (const Point3&, const Vector3& dir_v, Point2&) ;
  bool inline is_point_between_camera_bounds (const Point2&) const;

  std::vector <Mesh*> meshes_vector;

  inline Color calculate_lights (const Color& m_color, const Vector3& normal) const;
  void set_projection_data ();
  inline bool calculate_mesh_projection (const Face& face,
                                         const UV& uv,
                                         unsigned index);

  void generate_mesh_list (const std::vector<Mesh*>& meshes);

  inline void multithreaded_rasterize_mesh_list (unsigned init,
                                                 unsigned end);

  inline void multithreaded_rasterize_single_mesh (unsigned init,
                                                   unsigned end,
                                                   unsigned index,
                                                   const Mesh* aux_mesh);

  inline bool triangle_inside_screen (const Triangle2i& triangle);
  bool triangle_inside_camera(const Triangle2 &triangle);
  inline Triangle2i triangle_to_screen_space (const Triangle2& triangle, Triangle2i& t);
public:
  Projector(PerspectiveCamera* camera, World* world);
  void project();

  inline unsigned getNElementsToRender() {
    return n_elements_to_render;
  }

  inline std::vector<Triangle2i>* getElementsToRender () {
    return &elements_to_render;
  }

};

#endif // RASTERISER_H
