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
  unsigned width;
  unsigned height;
  unsigned depth;
  T* content;
public:
  Texture();
  Texture(unsigned w, unsigned h);

  // FIXME: This causes an error
  ~Texture() {
    //delete[] content;
  }

  void load(const std::string& filename);
  void write(const std::string& filename) const;

  void operator=(const Texture<T, D>& t);

  inline int get_width() const { return width; }
  inline int get_height() const { return height; }

  inline T get (const unsigned x,
                const unsigned y,
                const unsigned i = 0) const {

    return content[y * width * depth + x * depth + i];
  }

  inline T get (const unsigned index) const {
    return content[index];
  }

  inline void set (const unsigned x,
                   const unsigned y,
                   const T c,
                  const unsigned i = 0) {
    content[y * width * depth + x * depth + i] = c;
  }

  inline void set (const unsigned index, const T& c) const {
    content[index] = c;
  }

  void fill (T value);
};

#endif // TEXTURE_H
