#include "texture.h"

template <typename T, unsigned D>
Texture<T, D>::Texture() :
  width(0),
  height(0),
  depth(D),
  content(new T[1])
{}

template <typename T, unsigned D>
Texture<T, D>::Texture(unsigned w, unsigned h) :
  width(w),
  height(h),
  depth(D),
  content(new T[w * h * D])
{}

template<typename T, unsigned D>
void Texture<T, D>::fill(T value) {
  std::fill(content, content + (width * height * depth), value);
}

template <typename T, unsigned D>
void Texture<T, D>::load(const std::string& filename) {

  std::string token;
  std::ifstream file(filename);

  file >> token;                    // P3
  if (token[1] != '3') {
    std::cerr << "Error loading Texture " << filename <<
                 " incorrect depth value (" << depth << " vs " << token[1] << ")\n";
    return;
  }

  depth = token[1] - '0';
  file >> width;
  file >> height;

  file >> token;

  content = new T [height * width * depth];

  for (unsigned y = 0; y < height; y++) {
    for (unsigned x = 0; x < width; x++) {
      for (unsigned d = 0; d < depth; d++) {
        file >> token;
        T tmp = std::stoi(token);
        set(x, y, tmp, d);
      }
    }
  }
}

template <typename T, unsigned D>
void Texture<T, D>::write(const std::string& filename) const {
  std::ofstream file;
  file.open (filename);

  if (file.is_open()) {

    file << "P" << depth << "\n"
         << width << " "
         << height << " "
         << 255 << "\n";

    for (unsigned i = 0; i <   height; i++) {
      for (unsigned j = 0; j < width; j++) {
        for (unsigned d = 0; d < depth; d++) {
          file << unsigned(content[i * width * depth +
                                   j * depth + d]) << " ";
        }
        file << "\n";
      }
    }
    file.close();
  }
}

template class Texture<unsigned char, 3>; // Regular Texture
template class Texture<double, 1>;        // Z Buffer
template class Texture<unsigned long, 1>; // Triangle Buffer
