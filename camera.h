#ifndef CAMERA_H
#define CAMERA_H

#include "point3d.h"
#include "point2d.h"

class Camera : public Spatial {
private:
  Rect bounds;
  Point3 fuge;
  Vector3 vector_plane;
  Point3 point_plane;

public:

  Camera ();
  Camera (const Camera& cam) : Spatial (cam) {
    bounds = cam.bounds;
    fuge = cam.fuge;
    vector_plane = cam.vector_plane;
    point_plane = cam.point_plane;
  }

  inline Point3  get_fuge() const { return fuge; }
  inline Vector3 get_plane_vector() const { return vector_plane; }
  inline Vector3 get_plane_point() const { return point_plane; }
  inline Rect get_bounds() const { return bounds; }

  Camera* express_in_different_basis (const Basis3& new_basis) const;

  //Point2 adjust_rotation_basis (Point2 p);
  //virtual void rotate_z (double deg);
};

#endif // CAMERA_H

