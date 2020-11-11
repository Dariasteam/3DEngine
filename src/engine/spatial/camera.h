#ifndef CAMERA_H
#define CAMERA_H

#include "../math/point3d.h"
#include "../math/point2d.h"
#include "../planar/rect.h"
#include "spatial.h"

#define INFINITY_DISTANCE 10000000

class Camera : public Spatial {
protected:
  Vector3 vector_plane;
  RectF bounds {-1, -1, 1, 1};
  Point3 point_plane;
public:
  Camera(const Vector3& v_plane,
         const Point3& p_plane) :
    vector_plane (v_plane),
    point_plane (p_plane)
  {}

  Camera (const Camera& cam) :
    vector_plane (cam.vector_plane),
    bounds (cam.bounds),
    point_plane (cam.point_plane)
  {}

  inline const Vector3& get_plane_vector() const { return vector_plane; }
  inline const Point3&  get_plane_point() const { return point_plane; }
  inline const RectF& get_bounds() const { return bounds; }

  inline void set_plane_vector(const Vector3& p) { vector_plane = p; }
  inline void set_plane_point (const Point3& p)  { point_plane  = p; }
};

class ParallelCamera : public Camera {
private:

public:
  ParallelCamera(const Vector3& v_plane,
                 const Point3& p_plane) :
            Camera (v_plane, p_plane)
          {}

  ParallelCamera( const ParallelCamera& cam) :
    Camera(cam)
  {}
};


class PerspectiveCamera : public Camera {
public:  
  PerspectiveCamera (const Vector3& v_plane,
                     const Point3& p_plane) :
                Camera (v_plane, p_plane)
                {}

  PerspectiveCamera () :
    Camera (Vector3{0, 0, 1},
    Point3{0, 0, 3})
  {}

  PerspectiveCamera (const PerspectiveCamera& cam) :
    Camera (cam)
  {}  
};

#endif // CAMERA_H

