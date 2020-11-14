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
  Vector3 vector_plane;
  RectF bounds;
  Point3 point_plane {0, 0, 3};
  CommonBuffers& buffers;

  inline bool is_point_between_bounds (const Point2& p) const {
    return p.x() >= get_bounds().x       &
           p.x() <= get_bounds().width   &
           p.y() >= get_bounds().y       &
           p.y() <= get_bounds().height;
  };

  inline bool triangle_inside_camera (const Triangle& t) const {
    return is_point_between_bounds(t.a) &
           is_point_between_bounds(t.b) &
           is_point_between_bounds(t.c);
  };


  virtual bool calculate_mesh_projection (const Face& face,
                                         const UV& uv,
                                         unsigned thread_index) const = 0;
public:
  // FIXME: Do not allow this without adapting the plane orientation to vector
  Camera(const Vector3& v_plane,
         const RectF& b);

  Camera (const Camera& cam);

  inline const Vector3& get_plane_vector() const { return vector_plane; }
  inline const Point3&  get_plane_point() const { return point_plane; }
  inline const RectF& get_bounds() const { return bounds; }

  inline void set_bounds(const RectF& b) { bounds = b; }
  inline void set_plane_vector(const Vector3& p) { vector_plane = p; }
  inline void set_plane_point (const Point3& p)  { point_plane  = p; }  

  void project (const std::vector<Mesh*> meshes_vector) const;
};


#endif // CAMERA_H

