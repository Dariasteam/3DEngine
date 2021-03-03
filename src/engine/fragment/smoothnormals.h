#ifndef SMOOTHNORMALS_H
#define SMOOTHNORMALS_H

#include "fragmentshader.h"


class SmoothNormals: public FragmentOperation {
public:
  void operator () (unsigned pixel_index);
  Vector2 cut_point(const Point2& p, const Point3& v1,
                    const Point3& v2, const Point3& v3) const;
};

#endif // SMOOTHNORMALS_H
