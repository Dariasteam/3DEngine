#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "../math/point2i.h"

#include <vector>

struct face_uv {
  Point2i p;
  Point2i u;
  Point2i v;
};

class Material {
private:
  // Equivalence triangle -> 2 vectors and a point
  std::vector<face_uv> uvs;

  Texture albedo_channel;
public:
  Material();
};

#endif // MATERIAL_H
