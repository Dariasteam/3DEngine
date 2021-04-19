#ifndef NORMALS_H
#define NORMALS_H

#include "fragmentshader.h"

class FlatNormals : public FragmentOperation {
public:
  void operator () (unsigned pixel_index);
  virtual ~FlatNormals() {}
};

#endif // NORMALS_H
