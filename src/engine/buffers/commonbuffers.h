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
    triangles (50000),
    light_triangles (50000),

    triangle_indices (50000),
    l_triangle_indices (50000),

    n_renderable_triangles (0),
    n_l_renderable_triangles (0)
  {}
public:
  inline static CommonBuffers& get () {
    static CommonBuffers instance;
    return instance;
  }

  CommonBuffers (MultithreadManager const &) = delete;
  void operator= (CommonBuffers const &) = delete;

  Texture<unsigned long, 1> triangle_index_buffer;
  Texture<unsigned long, 1> l_triangle_index_buffer;

  Texture<double, 1> z_buffer;

  Texture<unsigned char, 3> normal_buffer;
  Texture<unsigned char, 3> screen_buffer;
//  Texture<bool, 1> lightmap_buffer;

  void set_dimension (unsigned w, unsigned h);;

  inline unsigned get_height() { return triangle_index_buffer.height(); }
  inline unsigned get_width() { return triangle_index_buffer.width(); }

  std::vector<Triangle> triangles;
  std::vector<Triangle> light_triangles;

  // FIXME: Should these be sets?
  std::vector<unsigned long> triangle_indices;
  std::vector<unsigned long> l_triangle_indices;

  void reset_z_buffer();

  unsigned n_renderable_triangles;
  unsigned n_l_renderable_triangles;
};

#endif // COMMONBUFFERS_H
