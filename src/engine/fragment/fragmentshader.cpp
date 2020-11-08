#include "fragmentshader.h"

FragmentShader::FragmentShader() {
  push_operation(new CalculateProjections());
  // FIXME: Asociate textures per mesh
  FragmentOperation::texture.load("line_texture.ppm");
}

void FragmentShader::operator()(std::vector<Triangle2i>* triangles) {
  unsigned n_pixels = CommonBuffers::get().get_height() *
                      CommonBuffers::get().get_width();

  // Set triangles
  FragmentOperation::triangles = triangles;
  FragmentOperation::texture_projectors.resize(triangles->size());
  FragmentOperation::matrices.resize(triangles->size());
  std::fill(FragmentOperation::matrices.begin(),
            FragmentOperation::matrices.end(),
            false);

  // Begin shading process
  MultithreadManager::get_instance().calculate_threaded(n_pixels,
                                                        [&](unsigned pixel_index) {

    if (CommonBuffers::get().z_buffer.get(pixel_index) >= INFINITY_DISTANCE) return;

    for (auto& operation : operations) {
      operation->operator()(pixel_index);
    }
  });
}

void FragmentShader::push_operation(FragmentOperation* op) {
  operations.push_back(op);
}

void CalculateProjections::operator()(unsigned pixel_index) {
  unsigned t_index = CommonBuffers::get().triangle_index_buffer.get(pixel_index);

  if (matrices[t_index])
    return;

  mtx.lock();
  matrices[t_index] = true;
  mtx.unlock();

  FragmentOperation::texture_projectors[t_index].generate_uv_projector(
    texture, (*triangles)[t_index], (*triangles)[t_index].uv);
}




std::vector<Triangle2i>* FragmentOperation::triangles = nullptr;
std::vector<TextureProjector> FragmentOperation::texture_projectors;
std::vector<bool> FragmentOperation::matrices;
Texture<unsigned char, 3> FragmentOperation::texture;
std::mutex CalculateProjections::mtx;
