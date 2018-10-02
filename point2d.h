#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>

struct Point2 {
  double x;
  double y;
};

struct Rect {
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

struct Triangle2 {
  Point2 a;
  Point2 b;
  Point2 c;
};

#endif // POINT2D_H


