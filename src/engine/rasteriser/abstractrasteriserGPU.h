#ifndef ABSTRACTRASTERISERGPU_H
#define ABSTRACTRASTERISERGPU_H

#include "abstractrasteriser.h"
#include "../../auxiliar/multithreadmanager.h"

#include <vector>
#include <fstream>
#include <algorithm>
#include <CL/cl.hpp>

class AbstractRasteriserGPU : public AbstractRasteriser {
private:

  void initialize_opencl ();
protected:

  cl::Device device;
  cl::Context context;
  cl::Program::Sources sources;
  cl::Program program;
  cl::CommandQueue queue;
  bool ready = false;

  cl::Buffer buffer_img;
  cl::Buffer buffer_z;
  cl::Kernel kernel;

/*
  inline bool triangle_inside_screen (const Triangle2i& triangle);
  inline Triangle2i triangle_to_screen_space (const Triangle2& triangle);
*/
  void load_kernel (std::string path, std::string kernel_name);

public:
  AbstractRasteriserGPU(World* w, Canvas* cv) :
    AbstractRasteriser (w, cv),
    buffer_img (context, CL_MEM_WRITE_ONLY, sizeof(unsigned char) * SCREEN_SIZE * SCREEN_SIZE * 3),
    buffer_z   (context, CL_MEM_WRITE_ONLY, sizeof(float) * SCREEN_SIZE * SCREEN_SIZE)
  {
    initialize_opencl ();
  }

  void rasterise (std::vector<Triangle2i>* triangles, unsigned sz) = 0;
};


#endif // ABSTRACTRASTERISERGPU_H
