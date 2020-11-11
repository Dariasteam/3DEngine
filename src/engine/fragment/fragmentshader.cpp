#include "fragmentshader.h"

FragmentShader::FragmentShader(const World* w) :
  buffers(CommonBuffers::get())
{
  push_operation(new CalculateProjections());

  // FIXME: Asociate textures per mesh
  FragmentOperation::texture.load("/home/darias/Desarrollo/3D/texture.ppm");
  FragmentOperation::normal_map.load("/home/darias/Desarrollo/3D/normal.ppm");
  FragmentOperation::world = w;
}

void FragmentShader::operator()() {
  unsigned n_pixels = buffers.get_height() * buffers.get_width();

  // Set triangles  
  FragmentOperation::texture_projectors.resize(buffers.triangles_size);
  FragmentOperation::matrices.resize(buffers.triangles_size);
  std::fill(FragmentOperation::matrices.begin(),
            FragmentOperation::matrices.end(),
            false);

  // Begin shading process  

  MultithreadManager::get_instance().calculate_threaded(n_pixels,
                                                        [&](unsigned pixel_index) {

    if (buffers.z_buffer.get(pixel_index) >= INFINITY_DISTANCE) return;

    for (auto& operation : operations) {
      operation->operator()(pixel_index);
    }
  });
}

void FragmentShader::push_operation(FragmentOperation* op) {
  operations.push_back(op);
}

void CalculateProjections::operator()(unsigned pixel_index) {
  unsigned t_index = buffers.triangle_index_buffer.get(pixel_index);

  if (matrices[t_index])
    return;

  mtx.lock();
  matrices[t_index] = true;
  mtx.unlock();

  FragmentOperation::texture_projectors[t_index].generate_uv_projector(
        buffers.triangles[t_index],
        buffers.triangles[t_index].uv);
}


std::vector<TextureProjector> FragmentOperation::texture_projectors;
std::vector<bool> FragmentOperation::matrices;
Texture<unsigned char, 3> FragmentOperation::texture;
Texture<unsigned char, 3> FragmentOperation::normal_map;
const World* FragmentOperation::world = nullptr;
std::mutex CalculateProjections::mtx;


FragmentOperation::FragmentOperation() :
  buffers (CommonBuffers::get())
{}
