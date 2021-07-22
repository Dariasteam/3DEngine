#include "../math/point3.h"
#include "../math/vector3.h"
#include "../math/vector3unitary.h"
#include "vertex.h"

struct Face {
  Vertex* a;
  Vertex* b;
  Vertex* c;

  Normal3 normal;

  /*
  Point3 a;
  Point3 b;
  Point3 c;

  Normal3 normal_a;
  Normal3 normal_b;
  Normal3 normal_c;
*/
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

/*
  inline Normal3& get_normal (unsigned i) {
    switch (i) {
      case 0:
        return normal_a;
      case 1:
        return normal_b;
      default:
        return normal_c;
    }
  }
*/
};

