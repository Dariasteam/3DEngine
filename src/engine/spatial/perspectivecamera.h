#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "camera.h"


class PerspectiveCamera : public Camera {
protected:
  bool calculate_mesh_projection (const Face& face,
                                          const UV& uv,
                                          unsigned thread_index) const;

  void calculate_cut_point (const Point3& vertex,
                                   Point2&) const;
public:
  PerspectiveCamera (const Vector3& v_plane,
                     const RectF& b);

  PerspectiveCamera (const PerspectiveCamera& cam);
};

#endif // PERSPECTIVECAMERA_H
