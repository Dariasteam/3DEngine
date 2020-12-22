#ifndef FRAGMENTSHADER_H
#define FRAGMENTSHADER_H

#include "../../auxiliar/multithreadmanager.h"
#include "../buffers/commonbuffers.h"
#include "../projector.h"
#include "../world.h"
#include "../planar/textureprojector.h"

#include "../math/point2.h"
#include "../math/point2d.h"
#include "../math/point3.h"
#include "../math/point3d.h"
#include "../planar/triangle.h"
#include "../spatial/camera.h"
#include "../world.h"
#include "../planar/texture.h"

#include <algorithm>
#include <list>

class FragmentOperation {
protected:

  CommonBuffers& buffers;

  inline Point2i pixel_index_to_screen_coordenates(unsigned pixel_index) {
    unsigned width = CommonBuffers::get().get_width();

    unsigned y = pixel_index / width;
    unsigned x = pixel_index  - (y * width);

    return Point2i(x, y);
  }

  inline unsigned get_triangle_index_at_pixel_index (unsigned pixel_index) {
    return buffers.triangle_index_surface.get(pixel_index);
  }


  inline Triangle& get_triangle_at_pixel_index(unsigned pixel_index) {
    return buffers.triangles[get_triangle_index_at_pixel_index(pixel_index)];
  }

  template <typename T>
  inline void clamp (T& c) const {
    c = std::max ((T)0, std::min(c, (T)255));
  }

  inline void clamp_color (Color& color) const {
    color.set_x(std::max (0.0, std::min(color.x(), 255.0)));
    color.set_y(std::max (0.0, std::min(color.y(), 255.0)));
    color.set_z(std::max (0.0, std::min(color.z(), 255.0)));
  }

public:
  static std::vector<TextureProjector> texture_projectors;
  static std::vector<TextureProjector> lightness_projectors;  
  static std::vector<bool> l_matrices;

  static Texture<unsigned char, 3> texture;
  static Texture<unsigned char, 3> normal_map;
public:
  virtual void operator()(unsigned pixel_index) = 0;
  FragmentOperation();
};

class FragmentShader {
private:
  std::list<FragmentOperation*> operations;
  CommonBuffers& buffers;
public:
  FragmentShader();
  void operator() ();
  void generate_texture_projectors();
  void generate_light_projectors();
  void push_operation(FragmentOperation* op);
};

#endif // FRAGMENTSHADER_H
