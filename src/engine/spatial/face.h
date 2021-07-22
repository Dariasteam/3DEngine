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

    normal_local = normal;
  }
};

