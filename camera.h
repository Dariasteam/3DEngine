#ifndef CAMERA_H
#define CAMERA_H

#include "point3d.h"
#include "point2d.h"

class Camera : public Spatial {
private:
  Basis2 projected_basis;
  Rect bounds;

public:
  Point3 fuge;
  Vector3 vector_plane;
  Point3 point_plane;

  Camera();
  inline Point3  get_fuge() const { return fuge; }
  inline Vector3 get_plane_vector() const { return vector_plane; }
  inline Vector3 get_plane_point() const { return point_plane; }
  inline Rect get_bounds() const { return bounds; }
  inline Basis2 get_projected_basis () const { return projected_basis; }

  Camera* express_in_different_basis (const Basis3& new_basis) const;

  //Point2 adjust_rotation_basis (Point2 p);
  //virtual void rotate_z (double deg);
};

#endif // CAMERA_H

