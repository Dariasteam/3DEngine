#ifndef PARALLELCAMERA_H
#define PARALLELCAMERA_H

#include "camera.h"

class ParallelCamera : public Camera {
protected:
  virtual bool calculate_mesh_projection (const Face& face,
                                          const UV& uv,
                                          unsigned thread_index) const;
public:
  ParallelCamera(const Vector3& v_plane,
                 const RectF& b);

  ParallelCamera(const ParallelCamera& cam);
};

#endif // PARALLELCAMERA_H
