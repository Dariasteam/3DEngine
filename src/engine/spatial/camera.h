#ifndef CAMERA_H
#define CAMERA_H

#include "../planar/rect.h"
#include "../planar/triangle.h"
#include "../buffers/commonbuffers.h"
#include "spatial.h"
#include "mesh.h"

class Camera : public Spatial {
protected:
  //FIXME: Should we have two vectors? When rendering local vector is just
  //       0,0,1, since everything is in camera basis
  Vector3 local_vector_plane;
  Vector3 global_vector_plane;

  // FIXME: only use height and width?
  RectF bounds;

  Point3 local_point_plane {0, 0, 3};
  Point3 global_point_plane {0, 0, 3};

  std::vector<Triangle>* triangle_buffer;
  unsigned* n_triangles_buffer;
  std::vector<unsigned long>* t_indices;

  inline bool is_point_between_bounds (const Point3& p) const {
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

  virtual bool calculate_face_projection (const Face& face,
                                          const UV& uv,
                                          unsigned thread_index) const = 0;
public:
  // FIXME: Do not allow this without adapting the plane orientation to vector
  Camera(const Vector3& v_plane,
         const RectF& b);

  Camera (const Camera& cam);
  virtual ~Camera() {}

  inline const Vector3& get_plane_vector() const { return local_vector_plane; }
  inline const Point3&  get_plane_point() const { return local_point_plane; }
  inline const RectF& get_bounds() const { return bounds; }

  inline void set_bounds(const RectF& b) { bounds = b; }
  inline void set_plane_vector(const Vector3& p) { local_vector_plane = p; }
  inline void set_plane_point (const Point3& p)  { local_point_plane  = p; }

  inline void set_triangle_buffer (std::vector<Triangle>* t) { triangle_buffer = t; }
  inline void set_n_triangle_buffer (unsigned* i) { n_triangles_buffer = i;}
  inline void set_t_indices (std::vector<unsigned long>* t) { t_indices = t;}

  inline std::vector<Triangle>* get_triangle_buffer () const { return triangle_buffer;}
  inline unsigned get_n_triangle_buffer () const { return *n_triangles_buffer; }
  inline std::vector<unsigned long>* get_t_indices () const { return t_indices;}

  void project (const std::vector<Mesh*> meshes_vector) const;


  inline const Vector3& get_global_plane_vector() const { return global_vector_plane; }


  // FIXME: This is not a significative name
  void express_in_parent_basis (const Basis3& b) {
    Matrix m;
    MatrixOps::generate_basis_change_matrix(b, basis, m);
    Point3Ops::change_basis(m, local_vector_plane, global_vector_plane);
    Point3Ops::change_basis(m, local_point_plane, global_point_plane);
  }
};


#endif // CAMERA_H

