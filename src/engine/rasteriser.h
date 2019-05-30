#ifndef RASTERISER_H
#define RASTERISER_H

#include "camera.h"
#include "world.h"
#include "canvas.h"
#include "list"

#include <future>
#include <mutex>

#define N_THREADS 8

struct Auxiliar {
  Point3 a;
  Point3 b;
  Point3 c;

  Vector3 v1;
  Vector3 v2;
  Vector3 v3;
};

class Rasteriser {
private:
  std::mutex mtx;

  World* world;
  Camera* camera;
  Canvas* canvas;

  bool calculate_cut_point (const Point3&, const Vector3& dir_v, Point3&) const;
  bool inline is_point_between_camera_bounds (const Point2&) const;

  Point3  camera_fuge;
  Vector3 camera_plane_vector;
  Rect camera_bounds;
  Point3 camera_plane_point;
  std::vector<std::vector <Triangle2>> projected_elements;
  std::vector<Triangle2*> elements_to_render;

  std::vector <Mesh*> meshes_vector;

  Color calculate_lights (const Color& m_color, const Face3& face) const;
  void set_rasterization_data ();
  inline bool calculate_mesh_projection (const Face3& face,
                                         const Matrix3& M2,
                                         std::vector<Triangle2>& triangles,
                                         unsigned index,
                                         Auxiliar& aux,
                                         const Color& color);
  void generate_mesh_list (const std::vector<Mesh*>& meshes);
public:
  Rasteriser(Canvas* canvas, Camera* camera, World* world);
  void rasterise();

};

#endif // RASTERISER_H
