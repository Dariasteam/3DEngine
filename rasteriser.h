#ifndef RASTERISER_H
#define RASTERISER_H

#include "camera.h"
#include "world.h"
#include "canvas.h"

class Rasteriser {
private:
  World* world;
  Camera* camera;
  Canvas* canvas;

  bool calculate_cut_point (Point3, Point3&);
  bool is_point_between_camera_bounds (const Point2&);

  Point3  camera_fuge;  
  Vector3 camera_plane_vector;  
  Rect camera_bounds;
  Point3 camera_plane_point;

public:
  Rasteriser(Canvas* canvas, Camera* camera, World* world);
  void rasterize();

};

#endif // RASTERISER_H
