#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include "../math/point2.h"

struct UV {
  Point2 p;
  Vector2 u;
  Vector2 v;
};

class Texture {
protected:
  unsigned width;
  unsigned height;
  unsigned char* content;
public:
  Texture();
  Texture(unsigned w, unsigned h);
  virtual void load(const std::string& filename) = 0;
  virtual void write(const std::string& filename) const;

  inline int get_width() const { return width; }
  inline int get_height() const { return height; }
};

class Texture1C : public Texture {
public:
  void load(const std::string& filename);
  void write(const std::string& filename) const;

  inline unsigned char get (const unsigned x,
                            const unsigned y,
                            const unsigned i) const {

    return content[y * width + x + i];
  }

  inline unsigned char get_color (const unsigned x,
                                  const unsigned y) const {
    return content[y * width * COLOR_DEPTH + x * COLOR_DEPTH];
  }

  inline void set (const unsigned x,
                   const unsigned y,
                   const unsigned i,
                   const unsigned char c) {
    content[y * width * COLOR_DEPTH + x * COLOR_DEPTH + i] = c;
  }


  Texture1C();
  Texture1C(unsigned w, unsigned h);
};

class Texture3C : public Texture {
private:
  const unsigned COLOR_DEPTH = 3;
public:
  void load(const std::string& filename);
  void write(const std::string& filename) const;

  inline unsigned char get (const unsigned x,
                            const unsigned y,
                            const unsigned i) const {

    return content[y * width * COLOR_DEPTH + x * COLOR_DEPTH + i];
  }

  inline Color888 get_color (const unsigned x,
                             const unsigned y) const {
    return Color888 {
      content[y * width * COLOR_DEPTH + x * COLOR_DEPTH + 1],
      content[y * width * COLOR_DEPTH + x * COLOR_DEPTH + 2],
      content[y * width * COLOR_DEPTH + x * COLOR_DEPTH + 3]
    };
  }

  inline void set (const unsigned x,
                   const unsigned y,
                   const unsigned i,
                   const unsigned char c) {
    content[y * width * COLOR_DEPTH + x * COLOR_DEPTH + i] = c;
  }


  Texture3C();
  Texture3C(unsigned w, unsigned h);
};

#endif // TEXTURE_H
