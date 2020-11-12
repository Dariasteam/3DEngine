#include "parallelcamera.h"

bool ParallelCamera::calculate_cut_point(const Point3&,
                                         const Vector3& dir_v,
                                         Point2&) const
{
  return true;
}

ParallelCamera::ParallelCamera(const Vector3& v_plane,
                               const Point3& p_plane) :
  Camera (v_plane, p_plane)
{}

ParallelCamera::ParallelCamera(const ParallelCamera& cam) :
  Camera(cam)
{}
