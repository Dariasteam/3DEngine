#ifndef CAMERA_H
#define CAMERA_H

#include "point3d.h"
#include "point2d.h"


class Camera : public Spatial {

  struct Parameters {
    Point3 fuge;
    Vector3 vector_plane;
    Point3 point_plane;
  };

private:
  Rect bounds;

  Parameters local_parameters;
  Parameters global_parameters;
public:

  Camera ();
  Camera (const Camera& cam) : Spatial (cam) {

    local_parameters = {
      cam.local_parameters.fuge,
      cam.local_parameters.vector_plane,
      cam.local_parameters.point_plane
    };

    bounds = cam.bounds;    
  }

  inline Point3  get_fuge() const { return global_parameters.fuge; }
  inline Vector3 get_plane_vector() const { return global_parameters.vector_plane; }
  inline Point3  get_plane_point() const { return global_parameters.point_plane; }
  inline Rect get_bounds() const { return bounds; }

  void translate (const Vector3& vec) {
    position += vec;
  }

  void apply_transform ();
};

#endif // CAMERA_H

