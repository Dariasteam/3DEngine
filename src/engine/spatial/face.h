#include "../math/point3.h"
#include "../math/vector3.h"
#include "../math/vector3unitary.h"
#include "vertex.h"

// FIXME: Declare copy constructor?
struct Face {
  Vertex* a;
  Vertex* b;
  Vertex* c;

  Normal3 normal_local;
  Normal3 normal_global;

  inline const Vertex* operator[] (unsigned i) const {
    switch (i) {
      case 0:
        return a;
      case 1:
        return b;
      default:
        return c;
    }
  }

  inline Vertex* operator[] (unsigned i) {
    switch (i) {
      case 0:
        return a;
      case 1:
        return b;
      default:
        return c;
    }
  }

  void generate_local_normal () {
    const Vector3& u = Vector3::create_vector(b->point_local, a->point_local);
    const Vector3& v = Vector3::create_vector(a->point_local, c->point_local);

    Vector3 tmp = Vector3::cross_product(v, u);
    tmp.normalize();
    normal_local = Normal3(tmp);
  }
};

