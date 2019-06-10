#ifndef RASTERISER_H
#define RASTERISER_H

#include "camera.h"
#include "world.h"
#include "canvas.h"
#include "list"

#include <future>
#include <mutex>

class Rasteriser {
private:
  std::mutex mtx;

  World* world;
  Camera* camera;
  Canvas* canvas;
  bool double_faces = false;

  bool inline calculate_cut_point (const Point3&, const Vector3& dir_v, Point3&) const;
  bool inline is_point_between_camera_bounds (const Point2&) const;

  std::vector<std::vector <Triangle2>> projected_elements;    

  std::vector<Triangle2> elements_to_render_buff_a;
  std::vector<Triangle2> elements_to_render_buff_b;

  std::vector <Mesh*> meshes_vector;

  inline Color calculate_lights (const Color& m_color, const Face3& face) const;
  void set_rasterization_data ();
  inline bool calculate_mesh_projection (const Face3& face,                                         
                                         std::vector<Triangle2>& triangles,
                                         unsigned index,
                                         const Color& color);
  void generate_mesh_list (const std::vector<Mesh*>& meshes);
public:
  Rasteriser(Canvas* canvas, Camera* camera, World* world);
  void rasterise();

};

#endif // RASTERISER_H
