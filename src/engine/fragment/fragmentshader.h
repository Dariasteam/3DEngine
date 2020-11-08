#ifndef FRAGMENTSHADER_H
#define FRAGMENTSHADER_H

#include "../../auxiliar/multithreadmanager.h"
#include "../rasteriser/abstractrasteriser.h"
#include "../projector.h"
#include "../world.h"
#include "../planar/textureprojector.h"

#include "../math/point2.h"
#include "../math/point2d.h"
#include "../math/point3.h"
#include "../math/point3d.h"
#include "../planar/triangle.h"
#include "../camera.h"
#include "../world.h"
#include "../planar/texture.h"

#include <algorithm>
#include <list>

// FIXME: Generate all texture projection matrixes previously

class FragmentOperation {
protected:
  inline Point2i pixel_index_to_coordenates(unsigned pixel_index) {
    unsigned width = CommonBuffers::get().get_width();

    unsigned y = pixel_index / width;
    unsigned x = pixel_index  - (y * width);

    return Point2i(x, y);
  }

  inline unsigned get_triangle_index_at_pixel_index (unsigned pixel_index) {
    return CommonBuffers::get().triangle_index_buffer.get(pixel_index);
  }


  inline Triangle2i& get_triangle_at_pixel_index(unsigned pixel_index) {
    return (*triangles)[get_triangle_index_at_pixel_index(pixel_index)];
  }

public:
  static const World* world;
  static std::vector<Triangle2i>* triangles;
  static std::vector<TextureProjector> texture_projectors;
  static std::vector<bool> matrices;
  static Texture<unsigned char, 3> texture;
  static Texture<unsigned char, 3> normal_map;
public:
  virtual void operator()(unsigned pixel_index) = 0;
};

class CalculateProjections : public FragmentOperation {
  static std::mutex mtx;
  void operator()(unsigned pixel_index);
};


class FragmentShader {
private:
  std::list<FragmentOperation*> operations;
public:
  FragmentShader(const World* w);
  void operator() (std::vector<Triangle2i>* triangles);
  void push_operation(FragmentOperation* op);
};

#endif // FRAGMENTSHADER_H
