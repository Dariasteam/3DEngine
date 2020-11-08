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

/*
template <typename T>
class Texture1C : public Texture<T> {
public:
  void load(const std::string& filename);
  void write(const std::string& filename) const;

  inline T get (const unsigned x,
                const unsigned y) const {

    return content[y * width + x];
  }

  inline void set (const unsigned x,
                   const unsigned y,
                   const T c) {
    content[y * width +  x] = c;
  }

  Texture1C();
  Texture1C(unsigned w, unsigned h);
};

template <typename T>
class Texture3C : public Texture<T> {
public:
  void load(const std::string& filename);
  void write(const std::string& filename) const;

  inline Color888 get_color (const unsigned x,
                             const unsigned y) const {
    return Color888 {
      content[y * width * depth + x * depth + 1],
      content[y * width * depth + x * depth + 2],
      content[y * width * depth + x * depth + 3]
    };
  }

  Texture3C();
  Texture3C(unsigned w, unsigned h);
};
*/
#endif // TEXTURE_H
