#include "fragmentshader.h"

FragmentShader::FragmentShader() {}

void FragmentShader::operator()(std::vector<Triangle2i>* triangles) {
  unsigned n_pixels = CommonBuffers::get().get_height() *
                      CommonBuffers::get().get_width();
  for (auto& operation : operations) {
    operation->threaded_operation (triangles, n_pixels);
  }
}

void FragmentShader::push_operation(FragmentOperation* op) {
  operations.push_back(op);
}

void FragmentOperation::threaded_operation(std::vector<Triangle2i>* t, unsigned n_pixels) {
  triangles = t;
  MultithreadManager::get_instance().calculate_threaded(n_pixels, [&](unsigned i) {
    this->operator()(i);
  });
}
