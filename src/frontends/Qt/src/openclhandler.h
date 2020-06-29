#ifndef OPENCLHANDLER_H
#define OPENCLHANDLER_H

#include <vector>
#include <fstream>
#include <algorithm>
#include <CL/cl.hpp>

class OpenClHandler {
private:
  void initialize_opencl ();
  cl::Device device;
  cl::Context context;
  cl::Program::Sources sources;
  cl::Program program;
  cl::CommandQueue queue;
  bool ready = false;

  cl::Kernel kernel;

  void load_kernel (std::string path, std::string kernel_name);

public:
  OpenClHandler();
};

#endif // OPENCLHANDLER_H
