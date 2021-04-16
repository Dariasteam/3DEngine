#include "perspectivecamera.h"

PerspectiveCamera::PerspectiveCamera(const Vector3& v_plane,
                                     const RectF& b) :
  Camera (v_plane, b)
{}

PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera& cam) :
  Camera (cam)
{}

bool PerspectiveCamera::calculate_face_projection(const Face& face,
                                                  const UV& uv,
                                                  unsigned index) const {  
  auto& tmp_triangle = buffers.triangles[index];

  // 1. Check normal of the face is towards camera, do not check angle,
  // only if it's bigger than 90º insteads  
  if ((face.normal * face.a) >= 0) return false;

  // 2. Calculate distance to camera
  tmp_triangle.a.Z = Vector3::vector_module(face.a);
  tmp_triangle.b.Z = Vector3::vector_module(face.b);
  tmp_triangle.c.Z = Vector3::vector_module(face.c);  

  double z_min = std::min({tmp_triangle.a.Z,
                           tmp_triangle.b.Z,
                           tmp_triangle.c.Z});
  if (z_min >= INFINITY_DISTANCE) return false;

  // 3. Calculate intersection points with the plane
  calculate_cut_point(face.a, tmp_triangle.a);
  calculate_cut_point(face.b, tmp_triangle.b);
  calculate_cut_point(face.c, tmp_triangle.c);

  if (!triangle_inside_camera(tmp_triangle)) return false;

  // 4. Copy normals. Global normal used to calculate
  // depth of each pixel with plane equation
  tmp_triangle.normal   = face.normal;
  tmp_triangle.normal_a = {face.normal_a.X, face.normal_a.Y};
  tmp_triangle.normal_b = {face.normal_b.X, face.normal_b.Y};
  tmp_triangle.normal_c = {face.normal_c.X, face.normal_c.Y};  

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
void PerspectiveCamera::calculate_cut_point(const Point3& vertex,
                                            Point3& result) const {
  const Vector3& dir_v = vertex;
  // Since we are in camera space, the vertex is also the director vector of the line

  // Calc cut point line - plane

  // Since we are in camera space plane normal vector is (0, 0, 1)
  const double C = get_plane_vector().z();

  const double D = get_plane_point().z() * C;

  const double& a = vertex.x();
  const double& c = vertex.y();
  const double& e = vertex.z();

  const double& b = dir_v.x();
  const double& d = dir_v.y();
  const double& f = dir_v.z();

  double T1 {- D + C*e};
  double T2 {- C*f};
  double beta = T1 / T2;

  // Intersection in camera coordinates
  result.set_x(a + b * beta);
  result.set_y(-(c + d * beta));    
}
