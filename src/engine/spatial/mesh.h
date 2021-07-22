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

  std::list<Mesh*> express_in_parents_basis (const Basis3& new_basis,
                                             const Point3& camera_translation);

  void inline change_basis_multithreaded (const std::list<Mesh*>& mesh_list,
                                          const Basis3& new_basis,
                                          const Point3& position);

  void apply_rotations ();
};

#endif // MESH_H
