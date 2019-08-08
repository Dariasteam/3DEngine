#include "../math/point3.h"
#include "../math/vector3.h"

struct Face {
  Point3 a;
  Point3 b;
  Point3 c;
  Vector3 normal;

  Vector3 normal_a;
  Vector3 normal_b;
  Vector3 normal_c;

  inline const Point3& operator[] (unsigned i) const {
    switch (i) {
      case 0:
        return a;
      case 1:
        return b;
      default:
        return c;
    }
  }

  inline Point3& operator[] (unsigned i) {
    switch (i) {
      case 0:
        return a;
      case 1:
        return b;
      default:
        return c;
    }
  }

  inline Vector3& get_normal (unsigned i) {
    switch (i) {
      case 0:
        return normal_a;
      case 1:
        return normal_b;
      default:
        return normal_c;
    }
  }

  void generate_normal () {
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

    normal = {X, Y, Z};
    normal.normalize();

    normal_a = normal;
    normal_b = normal;
    normal_c = normal;
  }

};

