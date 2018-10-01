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

  Point2 calculate_cut_point (Point3);
  bool is_point_between_camera_bounds (Point2);

  Point3  camera_fuge;
  Vector3 camera_plane;
  Rect camera_bounds;

public:
  Rasteriser(Canvas* canvas, Camera* camera, World* world);
  void rasterize();

};

#endif // RASTERISER_H
