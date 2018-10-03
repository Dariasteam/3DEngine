#ifndef CAMERA_H
#define CAMERA_H

#include "point3d.h"
#include "point2d.h"

class Camera : public Spatial {
private:

  Rect bounds;
  void apply_matrix (const Matrix3& matrix);

public:
  Point3 fuge;
  Vector3 vector_plane;
  Point3 point_plane;

  Camera();
  inline Point3  get_fuge() { return fuge; }
  inline Vector3 get_plane_vector() { return vector_plane; }
  inline Vector3 get_plane_point() { return point_plane; }
  inline Rect get_bounds() { return bounds; }

  Camera* express_in_different_basis (Basis3 new_basis);
};

#endif // CAMERA_H

