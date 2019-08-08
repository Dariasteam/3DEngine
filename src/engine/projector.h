#ifndef RASTERISER_H
#define RASTERISER_H

#include "camera.h"
#include "world.h"
#include "canvas.h"
#include "multithreadmanager.h"

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

  std::vector<Triangle2> elements_to_render;

  bool inline calculate_cut_point (const Point3&, const Vector3& dir_v, Point3&) const;
  bool inline is_point_between_camera_bounds (const Point2F&) const;

  std::vector <Mesh*> meshes_vector;

  inline Color calculate_lights (const Color& m_color, const Vector3& normal) const;
  void set_projection_data ();
  inline bool calculate_mesh_projection (const Face3& face,                                         
                                         std::vector<Triangle2>& triangles,
                                         const Color& color);

  void generate_mesh_list (const std::vector<Mesh*>& meshes);

  void paint_triangle (const Triangle2& triangle, std::vector<std::vector<Color888>>* screen_buffer);

  inline void raster_triangle (const Triangle2& triangle, std::vector<std::vector<Color888>>* screen_buffer);
  inline double get_y (const Point2F& u, const Vector2& v, double x);
  inline double get_x (const Point2F& u, const Vector2& v, double y);


  inline void multithreaded_rasterize_mesh_list (unsigned init,
                                                 unsigned end);

  inline void multithreaded_rasterize_single_mesh (unsigned init,
                                                   unsigned end,
                                                   const Mesh* aux_mesh);

  inline bool triangle_inside_screen (const Triangle2& triangle);
  inline Triangle2 triangle_to_screen_space (const Triangle2F& triangle);
public:
  Projector(Camera* camera, World* world);
  std::vector<Triangle2>& project();


};

#endif // RASTERISER_H
