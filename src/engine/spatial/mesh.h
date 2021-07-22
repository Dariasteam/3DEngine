#ifndef MESH_H
#define MESH_H

#include "../../auxiliar/multithreadmanager.h"
#include "../planar/texture.h"
#include "../math/point3.h"
#include "../math/vector3.h"
#include "../math/matrix3.h"
#include "spatial.h"
#include "face.h"
#include "vertex.h"

#include <future>
#include <functional>
#include <list>
#include <vector>

struct Mesh : public Spatial {
  std::vector<Vertex> vertices;
  std::vector<Face> faces;

  // FIXME: Delete this
  /*
  std::vector<Face> local_coordenates_faces;
  std::vector<Face> global_coordenates_faces;
  */

  std::vector<Mesh*> nested_meshes;

  std::vector<UV> uv_per_face;
  RGBTexture texture;

  Mesh () {}
  ~Mesh () {}

  Mesh (unsigned i) :
    vertices(i)
  {}

  Mesh (const Mesh& m) :
    Spatial(m.basis, m.position),
    vertices(m.vertices),
    faces(m.faces),
    uv_per_face(m.uv_per_face)
  {}

  void add_nested_mesh (Mesh* mesh) {
    nested_meshes.push_back(mesh);
  }

  const std::vector<Mesh*>& get_nested_meshes () const {
    return nested_meshes;
  }

  void generate_normals () {
    for (auto& face : faces) {
      const auto a = face.a->point_local;
      const auto b = face.b->point_local;
      const auto c = face.c->point_local;

      const Vector3& u = Vector3::create_vector(b, a);
      const Vector3& v = Vector3::create_vector(a, c);

      double X = (v.y() * u.z() - v.z() * u.y());
      double Y = (v.z() * u.x() - v.x() * u.z());
      double Z = (v.x() * u.y() - v.y() * u.x());

      /*
      double D = a.z() * v.y() * u.x() +
                a.x() * v.z() * u.y() +
                a.y() * v.x() * u.z() -
                a.x() * v.y() * u.z() -
                a.y() * v.z() * u.x() -
                a.z() * v.x() * u.y();
      */

      Vector3 tmp = {X, Y, Z};
      tmp.normalize();
      auto normal = Normal3(tmp);

      face.normal = normal;

      // FIXME: Delete this
      /*
      face.a->normal = normal;
      face.b->normal = normal;
      face.c->normal = normal;
      */
    }
  }

  inline std::vector<Normal3*> get_adjacent_vertices (Point3& p,
                                                      unsigned from,
                                                      std::vector<bool>& vertex_normals);

  void generate_data ();

  std::list<Mesh*> express_in_parents_basis (const Basis3& new_basis,
                                             const Point3& camera_translation);

  void inline change_basis_multithreaded (const std::list<Mesh*>& mesh_list,
                                          const Basis3& new_basis,
                                          const Point3& position);

  void inline change_basis (const std::list<Mesh*> mesh_list,
                            const Basis3& new_basis,
                            const Point3& camera_translation,
                            bool update_rotation,
                            bool update_translation);

  void inline apply_translation_part (const Vector3& translation,
                                      unsigned from, unsigned to);


  void apply_rotations ();

  void inline change_basis_part (const Matrix3& basis_changer,
                                 const Vector3& translation,
                                 unsigned from, unsigned to);
};

#endif // MESH_H
