#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include "../math/point2.h"

// Fixme use a triangle instead?
struct UV {
  Point2 p;
  Vector2 u;
  Vector2 v;
};

template <typename T, unsigned D>
class Texture {
protected:
  unsigned w;
  unsigned h;
  unsigned d;
  T* content;
public:
  Texture();
  Texture(unsigned w, unsigned h);
  Texture(unsigned w, unsigned h, T value);

  // FIXME: This causes an error
  ~Texture() {
    delete[] content;
  }

  void load(const std::string& filename);
  void write(const std::string& filename) const;

  void operator=(const Texture<T, D>& t);

  inline unsigned width() const { return w; }
  inline unsigned height() const { return h; }
  inline unsigned depth() const { return d; }

  inline T get (const unsigned x,
                const unsigned y,
                const unsigned i = 0) const {

    return content[y * w * d + x * d + i];
  }

  inline T get (const unsigned index) const {
    return content[index];
  }

  inline void set (const unsigned x,
                   const unsigned y,
                   const T c,
                  const unsigned i = 0) {
    content[y * w * d + x * d + i] = c;
  }

  inline void set (const unsigned index, const T& c) const {
    content[index] = c;
  }

  void fill (T value);
};

using NormalBuffer        = Texture<unsigned char, 3>;
using ZBuffer             = Texture<double, 1>;
using LightMapBuffer      = Texture<bool, 1>;
using TriangleIndexBuffer = Texture<unsigned long, 1>;
using RGBTexture          = Texture<unsigned char, 3>;
using RGBATexture         = Texture<unsigned char, 4>;

#endif // TEXTURE_H
