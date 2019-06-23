#ifndef RASTERISER_H
#define RASTERISER_H

#include "camera.h"
#include "world.h"
#include "canvas.h"
#include "list"

#include <future>
#include <mutex>
#include <fstream>

class Rasteriser {
private:
  std::mutex mtx;

  World* world;
  Camera* camera;
  Canvas* canvas;
  bool double_faces = false;
  unsigned screen_size = 500;

  std::vector<std::vector<Color888>> screen_buffer;

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
                                         const Color& color);

  void generate_mesh_list (const std::vector<Mesh*>& meshes);

  void generate_frame (const std::vector<Triangle2>* buff);
  void paint_triangle (const Triangle2& triangle);
  void paint_triangle_multithread (const Triangle2& triangle);

  /*   a___b
   *   |   |
   *   c___d
   *
   * */
  void paint_half_rect_ad_up (const iRect& r, const Color888& c);
  void paint_half_rect_ad_down (const iRect& r, const Color888& c);
  void paint_half_rect_cb_up (const iRect& r, const Color888& c);
  void paint_half_rect_cb_down (const iRect& r, const Color888& c);

  inline void raster_triangle_y (const Triangle2& triangle);
  inline void raster_triangle_x (const Triangle2& triangle);
  inline void raster_triangle (const Triangle2& triangle);
  inline void paint_line (const Point2& a, const Point2& b);

  inline double get_y (const Point2& u, const Vector2& v, double x);
  inline double get_x (const Point2& u, const Vector2& v, double y);


  inline void multithreaded_rasterize_mesh_list (unsigned init,
                                                 unsigned end,
                                                 std::vector<Triangle2>* buff);

  inline void multithreaded_rasterize_single_mesh (unsigned init,
                                                   unsigned end,
                                                   std::vector<Triangle2>* buff,
                                                   const Mesh* aux_mesh);

public:
  Rasteriser(Canvas* canvas, Camera* camera, World* world);
  void rasterise();

};

#endif // RASTERISER_H
