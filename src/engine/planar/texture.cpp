#include "texture.h"

Texture::Texture(unsigned w, unsigned h) :
  width(w),
  height(h),
  content(new unsigned char[w * h * COLOR_DEPTH])
{}

Texture::Texture() {}

void Texture::load(const std::string& filename) {

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

      content [i * width * COLOR_DEPTH + j * COLOR_DEPTH + 0] = r;  // b r g
      content [i * width * COLOR_DEPTH + j * COLOR_DEPTH + 1] = g;
      content [i * width * COLOR_DEPTH + j * COLOR_DEPTH + 2] = b;
    }
  }
}

void Texture::write(const std::string& filename) const {

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
