#ifndef TEXTUREPROJECTOR_H
#define TEXTUREPROJECTOR_H

#include "texture.h"
#include "../math/matrix2.h"
#include "vertex.h"

#include <iostream>

class TextureProjector {
  Texture screen;
public:
  TextureProjector();

  void project (const Texture& t,
                Point2i p,
                Point2i v,
                Point2i u);

  void write() const;
};

#endif // TEXTUREPROJECTOR_H
