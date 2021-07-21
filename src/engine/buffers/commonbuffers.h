#ifndef COMMONBUFFERS_H
#define COMMONBUFFERS_H

#include "../planar/texture.h"

#include "../math/point2.h"
#include "../math/point3.h"
#include "../planar/triangle.h"

#define INFINITY_DISTANCE 10000000

struct RasterTarget {
  std::vector<Triangle> triangles;
  std::vector<unsigned long> triangle_indices;
  unsigned n_renderable_triangles;
};

class CommonBuffers {
private:
  CommonBuffers () :
    triangles (500000),
    light_triangles (500000),

    triangle_indices (500000),
    l_triangle_indices (500000),

    is_triangle_ocluded (500000),

    n_renderable_triangles (0),
    n_l_renderable_triangles (0)
  {}
public:
  inline static CommonBuffers& get () {
    static CommonBuffers instance;
    return instance;
  }

  CommonBuffers (const CommonBuffers &) = delete;
  void operator= (CommonBuffers const &) = delete;

  Texture<unsigned long, 1> triangle_index_surface;
  Texture<unsigned long, 1> l_triangle_index_surface;

  Texture<double, 1> z_buffer;
  Texture<double, 1> z_light;

  // FIXME: Use a 2 depth buffer for normals?
  Texture<unsigned char, 3> normal_buffer;
  Texture<unsigned char, 3> screen_buffer;

  void set_dimension (unsigned w, unsigned h);

  inline unsigned get_height() { return triangle_index_surface.height(); }
  inline unsigned get_width() { return triangle_index_surface.width(); }

  std::vector<Triangle> triangles;
  std::vector<Triangle> light_triangles;

  std::vector<unsigned long> triangle_indices;
  std::vector<unsigned long> l_triangle_indices;

  // We can share this between directional light and camera
  // since the ocluded objects in directional_light space will be
  // black and the ones in camera space won't be rendered anyway
  std::vector<bool> is_triangle_ocluded;

  void reset_z_buffer();

  unsigned n_renderable_triangles;
  unsigned n_l_renderable_triangles;
};

#endif // COMMONBUFFERS_H
