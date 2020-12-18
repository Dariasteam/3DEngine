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
  Vector3 local_vector_plane;
  Vector3 global_vector_plane;

  RectF bounds;

  Point3 local_point_plane {0, 0, 3};
  Point3 global_point_plane {0, 0, 3};

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

  inline const Vector3& get_plane_vector() const { return local_vector_plane; }
  inline const Point3&  get_plane_point() const { return local_point_plane; }
  inline const RectF& get_bounds() const { return bounds; }

  inline void set_bounds(const RectF& b) { bounds = b; }
  inline void set_plane_vector(const Vector3& p) { local_vector_plane = p; }
  inline void set_plane_point (const Point3& p)  { local_point_plane  = p; }

  void project (const std::vector<Mesh*> meshes_vector) const;


  inline const Vector3& get_global_plane_vector() const { return global_vector_plane; }


  // FIXME: This is not a significative name
  void express_in_parent_basis (const Basis3& b) {
    Matrix m;
    MatrixOps::generate_basis_change_matrix(b, basis, m);
    Point3Ops::change_basis(m, local_vector_plane, global_vector_plane);
    Point3Ops::change_basis(m, local_point_plane, global_point_plane);

//    global_vector_plane = local_vector_plane;
  }
};


#endif // CAMERA_H

