#ifndef RECT_H
#define RECT_H

#include <cmath>

struct RectF {
  double x;
  double y;
  double height;
  double width;

  double size_x () {
    return std::sqrt(std::pow(x, 2) + std::pow(width, 2));
  }

  double size_y () {
    return std::sqrt(std::pow(x, 2) + std::pow(width, 2));
  }

  double area () {
    return size_x() * size_y();
  }
};

struct Rect {
  unsigned x;
  unsigned y;
  unsigned height;
  unsigned width;

  unsigned size_x () const {
    double v = std::sqrt(std::pow(x, 2) + std::pow(width, 2));
    return static_cast<unsigned>(std::round(v));
  }

  unsigned size_y () const {
    double v = std::sqrt(std::pow(x, 2) + std::pow(width, 2));
    return static_cast<unsigned>(std::round(v));
  }

  unsigned area ()  const {
    return size_x() * size_y();
  }
};

#endif // RECT_H
