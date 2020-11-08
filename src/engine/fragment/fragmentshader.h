#ifndef FRAGMENTSHADER_H
#define FRAGMENTSHADER_H

#include "../../auxiliar/multithreadmanager.h"
#include "../rasteriser/abstractrasteriser.h"
#include "../projector.h"

#include "../math/point2.h"
#include "../math/point2d.h"
#include "../math/point3.h"
#include "../math/point3d.h"
#include "../planar/triangle.h"
#include "../camera.h"
#include "../world.h"
#include "../planar/texture.h"

#include <list>

class FragmentOperation {
protected:
  std::vector<Triangle2i>* triangles;
public:
  virtual void operator()(unsigned pixel_index) const = 0;
  void threaded_operation(std::vector<Triangle2i>* t, unsigned n_pixels);
};

class FragmentShader {
private:
  std::list<FragmentOperation*> operations;
public:
  FragmentShader();
  void operator() (std::vector<Triangle2i>* triangles);
  void push_operation(FragmentOperation* op);
};

#endif // FRAGMENTSHADER_H
