#include "texture.h"

template <typename T, unsigned D>
Texture<T, D>::Texture() :
  w(0),
  h(0),
  d(D),
  content(new T[1])
{}

template <typename T, unsigned D>
Texture<T, D>::Texture(unsigned w, unsigned h, T value) :
  w(0),
  h(0),
  d(D),
  content(new T[w * h * D])
{
  fill(value);
}

template <typename T, unsigned D>
Texture<T, D>::Texture(unsigned w, unsigned h) :
  w(w),
  h(h),
  d(D),
  content(new T[w * h * D])
{}

template<typename T, unsigned D>
void Texture<T, D>::fill(T value) {

  auto& m = MultithreadManager::get_instance();
  double segment = double(w * h * d) / N_THREADS;

  m.calculate_threaded(N_THREADS, [&](unsigned i) {
    for (unsigned j = std::round(segment * i); j < std::round(segment * (i + 1)); j++) {
      content[j] = value;
    }
  });

  //std::fill(content, content + (width() * height() * depth()), value);
}

template <typename T, unsigned D>
void Texture<T, D>::load(const std::string& filename) {

  std::string token;
  std::ifstream file(filename);

  file >> token;                    // P3
  if (token[1] != '3') {
    std::cerr << "Error loading Texture " << filename <<
                 " incorrect depth() value (" << depth() << " vs " << token[1] << ")\n";
    return;
  }

  d = token[1] - '0';
  file >> w;
  file >> h;

  file >> token;

  content = new T [height() * width() * depth()];

  for (unsigned y = 0; y < height(); y++) {
    for (unsigned x = 0; x < width(); x++) {
      for (unsigned d = 0; d < depth(); d++) {
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

    file << "P" << depth() << "\n"
         << width() << " "
         << height() << " "
         << 255 << "\n";

    for (unsigned i = 0; i < height(); i++) {
      for (unsigned j = 0; j < width(); j++) {
        for (unsigned d = 0; d < depth(); d++) {
          file << unsigned(content[i * width() * depth() +
                                   j * depth() + d]) << " ";
        }
        file << "\n";
      }
    }
    file.close();
  }
}

template<typename T, unsigned D>
void Texture<T, D>::operator=(const Texture<T, D>& t) {
  w = t.width();
  h = t.height();
  d = t.depth();

  content = new T [width() * height() * depth()];

  for (unsigned i = 0; i < width() * height() * depth(); i++) {
    content[i] = t.get(i);
  }

}

template class Texture<unsigned char, 2>;  // Normal map
template class Texture<unsigned char, 3>; // Regular Texture
template class Texture<unsigned char, 4>; // Transparency Texture
template class Texture<double, 1>;        // Z Buffer
template class Texture<unsigned long, 1>; // Triangle Buffer
template class Texture<bool, 1>;          // Light mapper