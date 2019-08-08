#ifndef CAMERA_H
#define CAMERA_H

#include "math/point3d.h"
#include "math/point2d.h"
#include "planar/rect.h"
#include "spatial/spatial.h"

class Camera : public Spatial {
private:
  RectF bounds;
  Point3 fuge;
  Vector3 vector_plane;
  Point3 point_plane;
public:  
  Camera ();
  Camera (const Camera& cam) :
    Spatial (cam),
    bounds (cam.bounds),
    fuge(cam.fuge),
    vector_plane (cam.vector_plane),
    point_plane (cam.point_plane)
  {}

  inline Point3  get_fuge() const { return fuge; }
  inline Vector3 get_plane_vector() const { return vector_plane; }
  inline Point3  get_plane_point() const { return point_plane; }
  inline RectF get_bounds() const { return bounds; }
};

#endif // CAMERA_H

