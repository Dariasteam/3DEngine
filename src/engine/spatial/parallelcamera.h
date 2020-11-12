#ifndef PARALLELCAMERA_H
#define PARALLELCAMERA_H

#include "camera.h"

class ParallelCamera : public Camera {
protected:
  inline virtual bool calculate_cut_point (const Point3&,
                                           const Vector3& dir_v,
                                           Point2&) const;
public:
  ParallelCamera(const Vector3& v_plane,
                 const Point3& p_plane);

  ParallelCamera( const ParallelCamera& cam);
};

#endif // PARALLELCAMERA_H
