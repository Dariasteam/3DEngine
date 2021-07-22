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
};

