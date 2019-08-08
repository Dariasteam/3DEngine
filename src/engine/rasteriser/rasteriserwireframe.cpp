#include "rasteriserwireframe.h"

void drawLine (Point2i v1, Point2i v2, std::vector<std::vector<Color888> >* screen_buffer) {

  if (v1.y() == v2.y()) {      // horizontal line
    if (v1.x() > v1.x())
      std::swap(v1, v2);

    for (unsigned x = v1.x(); x < v2.x(); x++)
       (*screen_buffer)[v1.y()][x] = Color888{255, 255, 255};
    return;
  }

  if (v1.y() > v1.y())
    std::swap(v1, v2);

  if (v1.x() == v2.x()) {      // vertical line
    for (unsigned y = v1.x(); y < v2.x(); y++)
       (*screen_buffer)[y][v1.x()] = Color888{255, 255, 255};
    return;
  }

  double invslope = double(v2.x() - v1.x()) / double(v2.y() - v1.y());
  double min_x = v1.x();

  for (unsigned y = v1.y(); y < v2.y(); y++) {
    unsigned x = std::round(min_x);
    (*screen_buffer)[y][x] = Color888{255, 255, 255};
    min_x += invslope;
  }
}

void RasteriserWireframe::rasterize_triangle(Triangle2i& triangle, std::vector<std::vector<Color888> >* screen_buffer) {
  drawLine(triangle.a, triangle.b, screen_buffer);
  drawLine(triangle.a, triangle.c, screen_buffer);
  drawLine(triangle.c, triangle.b, screen_buffer);
}
