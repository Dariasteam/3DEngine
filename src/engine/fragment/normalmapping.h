#ifndef NORMALMAPPING_H
#define NORMALMAPPING_H

#include "fragmentshader.h"

class NormalMapping: public FragmentOperation {
public:
  void operator () (unsigned pixel_index);
};

#endif // NORMALMAPPING_H
