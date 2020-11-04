#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include "../math/point2.h"

#define COLOR_DEPTH 3

struct UV {
  Point2 p;
  Vector2 u;
  Vector2 v;
};

class Texture {
private:
  unsigned width;
  unsigned height;
  unsigned char* content;
public:
  Texture(unsigned w, unsigned h);
  Texture();
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

  inline unsigned get_width() const { return width; }
  inline unsigned get_height() const { return height; }

  void print();  

};

#endif // TEXTURE_H
