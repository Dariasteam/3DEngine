#include "parallelcamera.h"

ParallelCamera::ParallelCamera(const Vector3& v_plane,
                               const RectF& b) :
  Camera (v_plane, b)
{}

ParallelCamera::ParallelCamera(const ParallelCamera& cam) :
  Camera(cam)
{}

inline bool ParallelCamera::calculate_mesh_projection(const Face& face,
                                                      const UV& uv,
                                                      unsigned index) const {

  auto& tmp_triangle = buffers.triangles[index];

  // 1. Face to 2D triangle
  // FIXME: -x to prevent inverted image, is the error here or in perspective camera?
  tmp_triangle.a.X = -face.a.X;
  tmp_triangle.a.Y = face.a.Y;

  tmp_triangle.b.X = -face.b.X;
  tmp_triangle.b.Y = face.b.Y;

  tmp_triangle.c.X = -face.c.X;
  tmp_triangle.c.Y = face.c.Y;

  // 2. Check triangle between camera bounds
  if (!triangle_inside_camera(tmp_triangle)) return false;

  // 3. Check normal of the face is towards camera, do not check angle,
  // only if it's bigger than 90º instead
  bool angle_normal = (face.normal * face.a) < 0
                    | (face.normal * face.b) < 0
                    | (face.normal * face.c) < 0;
//  if (!angle_normal) return false;

  // FIXME: Use per vertex Z instead of triangle one
  // 4. Calculate distance to camera
  double mod_v1 = face.a.Z;
  double mod_v2 = face.b.Z;
  double mod_v3 = face.c.Z;

  double z_min = std::min({mod_v1, mod_v2, mod_v3});
  //double z_max = std::max({mod_v1, mod_v2, mod_v3});
  if (z_min > INFINITY_DISTANCE) return false;

  // 5. Copy normals FIXME: Use only vertex normals, not the global one
  tmp_triangle.normal   = face.normal;
  tmp_triangle.normal_a = face.normal_a;
  tmp_triangle.normal_b = face.normal_b;
  tmp_triangle.normal_c = face.normal_c;
  tmp_triangle.z_value  = z_min;

  // Set uv
  tmp_triangle.uv = uv;
  return true;
}
