#ifndef TEXTUREPROJECTORSHADER_H
#define TEXTUREPROJECTORSHADER_H

#include "fragmentshader.h"

class TexturePainter : public FragmentOperation {
public:
  void operator () (unsigned pixel_index) const;
};

#endif // TEXTUREPROJECTORSHADER_H
