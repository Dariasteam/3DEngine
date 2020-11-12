#ifndef CAMERA_H
#define CAMERA_H

#include "../math/point3d.h"
#include "../math/point2d.h"
#include "../planar/rect.h"
#include "spatial.h"
#include "../buffers/commonbuffers.h"
#include "mesh.h"

class Camera : public Spatial {
protected:
  CommonBuffers& buffers;
  Vector3 vector_plane;
  RectF bounds {-1, -1, 1, 1};
  Point3 point_plane;

  inline bool is_point_between_camera_bounds (const Point2& p) const;
  inline bool triangle_inside_camera (const Triangle& t) const;


  inline bool calculate_mesh_projection (const Face& face,
                                         const UV& uv,
                                         unsigned thread_index) const;

  inline virtual bool calculate_cut_point (const Point3&,
                                           const Vector3& dir_v,
                                           Point2&) const = 0;
public:
  Camera(const Vector3& v_plane,
         const Point3& p_plane);

  Camera (const Camera& cam);

  inline const Vector3& get_plane_vector() const { return vector_plane; }
  inline const Point3&  get_plane_point() const { return point_plane; }
  inline const RectF& get_bounds() const { return bounds; }

  inline void set_plane_vector(const Vector3& p) { vector_plane = p; }
  inline void set_plane_point (const Point3& p)  { point_plane  = p; }  

  void project (const std::vector<Mesh*> meshes_vector) const;
};


#endif // CAMERA_H

