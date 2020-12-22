#ifndef LIGHTNESS_H
#define LIGHTNESS_H

#include "fragmentshader.h"

class Lightness : public FragmentOperation {
public:
  void operator () (unsigned pixel_index);
};

#endif // LIGHTNESS_H
