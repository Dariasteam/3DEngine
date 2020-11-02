#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>

#define COLOR_DEPTH 3

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
