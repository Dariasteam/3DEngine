#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "camera.h"


class PerspectiveCamera : public Camera {
protected:
  inline virtual bool calculate_cut_point (const Point3&,
                                           const Vector3& dir_v,
                                           Point2&) const;
public:
  PerspectiveCamera (const Vector3& v_plane,
                     const Point3& p_plane);

  PerspectiveCamera ();
  PerspectiveCamera (const PerspectiveCamera& cam);
};

#endif // PERSPECTIVECAMERA_H
