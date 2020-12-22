#ifndef SMOOTHNORMALS_H
#define SMOOTHNORMALS_H

#include "fragmentshader.h"


class SmoothNormals: public FragmentOperation {
public:
  void operator () (unsigned pixel_index);
};

#endif // SMOOTHNORMALS_H
