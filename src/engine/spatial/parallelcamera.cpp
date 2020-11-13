#include "parallelcamera.h"

ParallelCamera::ParallelCamera(const Vector3& v_plane,
                               const Point3& p_plane) :
  Camera (v_plane, p_plane)
{}

ParallelCamera::ParallelCamera(const ParallelCamera& cam) :
  Camera(cam)
{}

// Calculate the intersection point between the camera plane and the
// line described by vertex and camera_fugue (dir_v)
/* Plane expresed as Ax + By + Cz + D = 0
 * Rect expresed as
 *  x = a + bß
 *  y = c + dß
 *  z = e + fß
 *
 * T1 = D + A*a + B*c + C*e
 * T2 = - A*b - B*d - C*f
 *
 * ß = T1 / T2
 *
 * */
inline bool ParallelCamera::calculate_cut_point(const Point3& vertex,
                                                   const Vector3& dir_v,
                                                   Point2& point) const {
  // Calc cut point line - plane
  const double A = get_plane_vector().x();  // Since we are in camera space this should be always (0, 0, 1)
  const double B = get_plane_vector().y();
  const double C = get_plane_vector().z();

  const double D = -(get_plane_point().x() * A +
                     get_plane_point().y() * B +
                     get_plane_point().z() * C);

  const double& a = vertex.x();
  const double& c = vertex.y();
  const double& e = vertex.z();

  const double& b = dir_v.x();
  const double& d = dir_v.y();
  const double& f = dir_v.z();

  // NOTE: Changed from -D to avoid render upside down
  double T1 {-D + A*a + B*c + C*e};
  double T2 {- A*b - B*d - C*f};

  bool return_value = true;
/*
  // Some vertices are behind camera
  if ((f < 0 && C > 0) || (f > 0 && C < 0)) {
    T1 = -T1;
    return_value = false;
  }
*/
  double parameter = T1 / T2;

  // Intersection in camera coordinates
  point.set_x(a + b * parameter);
  point.set_y(-(c + d * parameter));
  //point.set_z(e + f * parameter);

  return return_value;
}
