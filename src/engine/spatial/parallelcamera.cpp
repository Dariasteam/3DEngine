#include "parallelcamera.h"

ParallelCamera::ParallelCamera(const Vector3& v_plane,
                               const RectF& b) :
  Camera (v_plane, b)
{}

ParallelCamera::ParallelCamera(const ParallelCamera& cam) :
  Camera(cam)
{}

inline bool ParallelCamera::calculate_face_projection(const Face& face,
                                                      const UV& uv,
                                                      unsigned index) const {

  auto& tmp_triangle = buffers.triangles[index];

  // 1. Face to triangle
  tmp_triangle.a = face.a;
  tmp_triangle.b = face.b;
  tmp_triangle.c = face.c;

  if (face.normal.Z > 0) return false;

  // 2. Check triangle between camera bounds
  if (!triangle_inside_camera(tmp_triangle)) return false;

  // 3. Check normal of the face is towards camera, do not check angle,
  // only if it's bigger than 90º instead
  if ((face.normal.toVector3() * face.a) >= 0) return false;

  // 4. Calculate distance to camera
  double mod_v1 = face.a.Z;
  double mod_v2 = face.b.Z;
  double mod_v3 = face.c.Z;

  double z_min = std::min({mod_v1, mod_v2, mod_v3});

  if (z_min > INFINITY_DISTANCE) return false;

  // Set uv
  tmp_triangle.uv = uv;
  return true;
}
