#ifndef COMMONBUFFERS_H
#define COMMONBUFFERS_H

#include "../planar/texture.h"

#include "../math/point2.h"
#include "../math/point2d.h"
#include "../math/point3.h"
#include "../math/point3d.h"
#include "../planar/triangle.h"

#define INFINITY_DISTANCE 10000000

class CommonBuffers {
private:
  CommonBuffers () :
    triangles_size (0),
    triangles (50000)
  {}
public:
  inline static CommonBuffers& get () {
    static CommonBuffers instance;
    return instance;
  }

  Texture<unsigned long, 1> triangle_index_buffer;
  Texture<double, 1> z_buffer;
  Texture<unsigned char, 3> normal_buffer;
  Texture<unsigned char, 3> screen_buffer;

  void set_dimension (unsigned w, unsigned h);;

  inline unsigned get_height() { return triangle_index_buffer.get_height(); }
  inline unsigned get_width() { return triangle_index_buffer.get_width(); }

  std::vector<Triangle> triangles;
  unsigned triangles_size;

  void reset_z_buffer();
};

#endif // COMMONBUFFERS_H
