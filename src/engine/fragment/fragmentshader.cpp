#include "fragmentshader.h"

FragmentShader::FragmentShader() :
  buffers(CommonBuffers::get())
{
  //push_operation(new CalculateTextureProjections());
  //push_operation(new CalculateLightnessProjections());

  // FIXME: Asociate textures per mesh
  FragmentOperation::texture.load("/home/darias/Desarrollo/3D/texture.ppm");
  FragmentOperation::normal_map.load("/home/darias/Desarrollo/3D/normal.ppm");
}

void FragmentShader::generate_projection_matrices() {

}

void FragmentShader::operator()() {
  unsigned n_pixels = buffers.get_height() * buffers.get_width();

  // Prepare texture projection
  // FIXME Way too many matrices nad projectors
  FragmentOperation::texture_projectors.resize(buffers.triangles.size());
  FragmentOperation::t_matrices.resize(buffers.triangles.size());
  std::fill(FragmentOperation::t_matrices.begin(),
            FragmentOperation::t_matrices.end(),
            false);  


  // Prepare lightness projection
  FragmentOperation::lightness_projectors.resize(buffers.triangles.size());
  FragmentOperation::l_matrices.resize(buffers.triangles.size());
  std::fill(FragmentOperation::l_matrices.begin(),
            FragmentOperation::l_matrices.end(),
            false);

  // Begin shading process  
  MultithreadManager::get_instance().calculate_threaded(n_pixels,
                                                        [&](unsigned pixel_index) {

    // If too far away, paint sky color
    if (buffers.z_buffer.get(pixel_index) >= INFINITY_DISTANCE) {
      buffers.screen_buffer.set(pixel_index * 3 + 0, 255);
      buffers.screen_buffer.set(pixel_index * 3 + 1, 255);
      buffers.screen_buffer.set(pixel_index * 3 + 2, 255);
      return;
    }    

    for (auto& operation : operations) {
      operation->operator()(pixel_index);
    }
  });
}

void FragmentShader::push_operation(FragmentOperation* op) {
  operations.push_back(op);
}

// FIXME: Use the triangle_indices instead of shader?
void CalculateTextureProjections::operator()(unsigned pixel_index) {
  unsigned t_index = get_triangle_index_at_pixel_index(pixel_index);

  if (t_matrices[t_index])
    return;

  mtx.lock();
  t_matrices[t_index] = true;
  mtx.unlock();

  FragmentOperation::texture_projectors[t_index].generate_uv_projector(
        buffers.triangles[t_index],
        buffers.triangles[t_index].uv);
}

void CalculateLightnessProjections::operator()(unsigned pixel_index) {
  unsigned t_index   = buffers.triangle_index_buffer.get(pixel_index);

  if (l_matrices[t_index])
    return;

  // Check the triangle exist in the light render
  auto it1 = buffers.l_triangle_indices.begin();
  auto it2 = buffers.l_triangle_indices.begin() + buffers.n_l_renderable_triangles;
  if (std::find(it1,
                it2,
                t_index
                ) == it2) {
    std::cout << "NOP" << std::endl;
    return;
  }

  mtx.lock();
    l_matrices[t_index] = true;
  mtx.unlock();

  auto& t = buffers.light_triangles[t_index];
  UV fakeuv;

  fakeuv.p = t.a;
  fakeuv.u = t.b - t.a;
  fakeuv.v = t.c - t.a;

  FragmentOperation::lightness_projectors[t_index].generate_uv_projector(
        buffers.light_triangles[t_index],
        fakeuv);
}


std::vector<TextureProjector> FragmentOperation::texture_projectors;
std::vector<bool> FragmentOperation::t_matrices;

std::vector<TextureProjector> FragmentOperation::lightness_projectors;
std::vector<bool> FragmentOperation::l_matrices;

Texture<unsigned char, 3> FragmentOperation::texture;
Texture<unsigned char, 3> FragmentOperation::normal_map;

std::mutex CalculateTextureProjections::mtx;
std::mutex CalculateLightnessProjections::mtx;


FragmentOperation::FragmentOperation() :
  buffers (CommonBuffers::get())
{}
