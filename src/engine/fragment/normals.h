#ifndef NORMALS_H
#define NORMALS_H

#include "fragmentshader.h"

class Normals : public FragmentOperation {
public:
  void operator () (unsigned pixel_index) const;
};

#endif // NORMALS_H
