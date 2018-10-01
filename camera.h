#ifndef CAMERA_H
#define CAMERA_H

#include "point3d.h"
#include "point2d.h"

class Camera : public Spatial {
private:
  Point3 fuge;
  Vector3 plane;
  Rect bounds;
public:
  Camera();
  inline Point3  get_fuge() { return fuge; }
  inline Vector3 get_plane() { return plane; }
  inline Rect get_bounds() { return bounds; }
};

#endif // CAMERA_H

