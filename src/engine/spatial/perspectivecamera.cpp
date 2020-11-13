#include "perspectivecamera.h"

PerspectiveCamera::PerspectiveCamera(const Vector3& v_plane,
                                     const RectF& b) :
  Camera (v_plane, b)
{}

PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera& cam) :
  Camera (cam)
{}

inline bool PerspectiveCamera::calculate_mesh_projection(const Face& face,
                                                          const UV& uv,
                                                          unsigned index) const {

  auto& tmp_triangle = buffers.triangles[index];

  //Triangle2 tmp_triangle;

  // ONLY USEFUL IF USING DOUBLE FACES
  // 1. Check face is not behind camera. Since we are using camera basis
  // we only need z value of the plane. Also each point coordinate is also
  // it's vector

  // FIXME: this should not exist
  /*
  auto plane_distance = camera->get_plane_point().z();
  if (face.a.z() < plane_distance &&
      face.b.z() < plane_distance &&
      face.c.z() < plane_distance) return false;
      */

  // 2. Check normal of the face is towards camera, do not check angle,
  // only if it's bigger than 90º instead
  bool angle_normal = (face.normal * face.a) < 0
                    | (face.normal * face.b) < 0
                    | (face.normal * face.c) < 0;
  if (!angle_normal) return false;

  // 2. Calculate distance to camera
  double mod_v1 = Vector3::vector_module(face.a);
  double mod_v2 = Vector3::vector_module(face.b);
  double mod_v3 = Vector3::vector_module(face.c);

  double z_min = std::min({mod_v1, mod_v2, mod_v3});
  //double z_max = std::max({mod_v1, mod_v2, mod_v3});
  if (z_min > INFINITY_DISTANCE) return false;

  // 3. Calculate intersection points with the plane
  calculate_cut_point(face.a, tmp_triangle.a);
  calculate_cut_point(face.b, tmp_triangle.b);
  calculate_cut_point(face.c, tmp_triangle.c);

  tmp_triangle.z_value = z_min;

  if (!triangle_inside_camera(tmp_triangle)) return false;

  // 4. Copy normals FIXME: Use only vertex normals, not the global one
  tmp_triangle.normal   = face.normal;
  tmp_triangle.normal_a = face.normal_a;
  tmp_triangle.normal_b = face.normal_b;
  tmp_triangle.normal_c = face.normal_c;

  // Set uv
  tmp_triangle.uv = uv;
  return true;
}

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
inline void PerspectiveCamera::calculate_cut_point(const Point3& vertex,
                                                   Point2& result) const {
  const Vector3& dir_v = vertex;            // Since we are in camera space, the vertex is also the director vector of the line

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
  double parameter = T1 / T2;

  // Intersection in camera coordinates
  result.set_x(a + b * parameter);
  result.set_y(-(c + d * parameter));
  //point.set_z(e + f * parameter);
}
