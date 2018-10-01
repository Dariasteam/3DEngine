#ifndef POINT3D_H
#define POINT3D_H


#include <vector>

struct Point3 {
  double x;
  double y;
  double z;
};

struct Vector3 {
  double x;
  double y;
  double z;
};

struct Base {
  Vector3 x;
  Vector3 y;
  Vector3 z;
};

struct Spatial {
  Base b;
};

struct Line3 {
  Point3 point;
  Vector3 vector;
  double parameter;
};

struct Face3 {
  Point3 a;
  Point3 b;
  Point3 c;
};

struct Mesh {
  std::vector<Face3> faces;
};

struct SpatialVector : public Spatial{
  double x;
  double y;
  double z;
};

struct SpatialPoint : public Spatial{
  double x;
  double y;
  double z;
};

struct SpatialTriangle : public Spatial {
  Point3 v1;
  Point3 v2;
  Point3 v3;
};

struct SpatialLine : public Spatial {
  SpatialPoint a;
  SpatialPoint b;
};



#endif // POINT3D_H
