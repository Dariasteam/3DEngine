#ifndef MESH_H
#define MESH_H

#include "../../auxiliar/multithreadmanager.h"
#include "../planar/texture.h"
#include "../math/point3.h"
#include "../math/vector3.h"
#include "../math/matrix3.h"
#include "spatial.h"
#include "face.h"

#include <future>
#include <functional>
#include <list>

struct Mesh : public Spatial {
  std::vector<Face> local_coordenates_faces;
  std::vector<Face> global_coordenates_faces;

  std::vector<Mesh*> nested_meshes;

  std::vector<UV> uv_per_face;
  RGBTexture texture;

  Mesh () {}
  ~Mesh () {}

  Mesh (unsigned i) :
    local_coordenates_faces(i),
    global_coordenates_faces(i)
  {}

  Mesh (const Mesh& m) :
    Spatial(m.basis, m.position),
    local_coordenates_faces (m.local_coordenates_faces),
    global_coordenates_faces (m.global_coordenates_faces),
    uv_per_face(m.uv_per_face)
  {}

  void add_nested_mesh (Mesh* mesh) {
    nested_meshes.push_back(mesh);
  }

  const std::vector<Mesh*>& get_nested_meshes () const {
    return nested_meshes;
  }

  void generate_normals () {
    for (auto& face : local_coordenates_faces)
      face.generate_normal();
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
