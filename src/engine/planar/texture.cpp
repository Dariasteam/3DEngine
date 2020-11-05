#include "texture.h"

Texture::Texture(unsigned w, unsigned h) :
  width(w),
  height(h)
{}

Texture::Texture() :
  width(0),
  height(0)
{}

Texture1C::Texture1C() {}
Texture1C::Texture1C(unsigned w, unsigned h) : Texture (w, h) {
  content = new unsigned char[w * h];
}


Texture3C::Texture3C() {}
Texture3C::Texture3C(unsigned w, unsigned h) : Texture (w, h) {
  content = new unsigned char[w * h * COLOR_DEPTH];
}

void Texture3C::load(const std::string& filename) {

  std::string token;
  std::ifstream file(filename);

  file >> token;    // P3
  file >> width;
  file >> height;

  file >> token;

  content = new unsigned char [height * width * COLOR_DEPTH];

  for (unsigned i = 0; i < height; i++) {
    for (unsigned j = 0; j < width; j++) {
      unsigned char r;
      unsigned char g;
      unsigned char b;

      file >> token;
      r = std::stoi(token);

      file >> token;
      g = std::stoi(token);

      file >> token;
      b = std::stoi(token);

      content [i * width * COLOR_DEPTH + j * COLOR_DEPTH + 0] = b;  // b r g
      content [i * width * COLOR_DEPTH + j * COLOR_DEPTH + 1] = r;
      content [i * width * COLOR_DEPTH + j * COLOR_DEPTH + 2] = g;
    }
  }
}

void Texture3C::write(const std::string& filename) const {

  std::cout << "Start writing" << std::endl;

  std::ofstream file;
  file.open (filename);

  if (file.is_open()) {

    file << "P" << COLOR_DEPTH << "\n"
         << width << " "
         << height << " "
         << 255 << "\n";

    for (unsigned i = 0; i < height; i++) {
      for (unsigned j = 0; j < width; j++) {
        for (unsigned d = 0; d < COLOR_DEPTH; d++) {
          file << unsigned(content[i * width * COLOR_DEPTH +
                                   j * COLOR_DEPTH + d]) << " ";
        }
        file << "\n";
      }
    }
    file.close();
  }

  std::cout << "End writting" << std::endl;
}
