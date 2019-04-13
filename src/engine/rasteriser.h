#ifndef RASTERISER_H
#define RASTERISER_H

#include "camera.h"
#include "world.h"
#include "canvas.h"
#include "list"

#include <future>
#include <mutex>

#define N_THREADS 8

class Rasteriser {
private:
  std::mutex mtx;

  World* world;
  Camera* camera;
  Canvas* canvas;

  bool calculate_cut_point (const Point3&, Point3&) const;
  bool inline is_point_between_camera_bounds (const Point2&) const;

  Point3  camera_fuge;
  Vector3 camera_plane_vector;
  Rect camera_bounds;
  Point3 camera_plane_point;
  std::vector <Triangle2>* projected_elements;

  std::vector <Mesh*> meshes_vec;

  void set_rasterization_data ();
  void calculate_mesh_projection (const Mesh* const mesh,
                                  const Matrix3& M2);
  void generate_mesh_list (const std::vector<Mesh*>& meshes);
public:
  Rasteriser(Canvas* canvas, Camera* camera, World* world);
  void rasterize();

};

#endif // RASTERISER_H
