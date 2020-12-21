#include "fragmentshader.h"

FragmentShader::FragmentShader() :
  buffers(CommonBuffers::get())
{
  // FIXME: Asociate textures per mesh
  FragmentOperation::texture.load("/home/darias/Desarrollo/3D/star_texture.ppm");
  FragmentOperation::normal_map.load("/home/darias/Desarrollo/3D/normal.ppm");
}

void FragmentShader::operator()() {
  unsigned n_pixels = buffers.get_height() * buffers.get_width();


  generate_texture_projectors();
  generate_light_projectors();

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

void FragmentShader::generate_texture_projectors() {
  FragmentOperation::texture_projectors.resize(buffers.triangles.size());

  MultithreadManager::get_instance().calculate_threaded(buffers.n_renderable_triangles,
                                                        [&](unsigned i) {
    unsigned t_index = buffers.triangle_indices[i];
    auto& triangle = buffers.triangles[t_index];
    FragmentOperation::texture_projectors[t_index].generate_uv_projector(triangle, triangle.uv);
  });
}

void FragmentShader::generate_light_projectors() {
  FragmentOperation::lightness_projectors.resize(buffers.light_triangles.size());
  FragmentOperation::l_matrices.resize(buffers.light_triangles.size());

  std::fill(FragmentOperation::l_matrices.begin(),
            FragmentOperation::l_matrices.end(),
            false);

  MultithreadManager::get_instance().calculate_threaded(buffers.n_l_renderable_triangles,
                                                        [&](unsigned i) {
    unsigned t_index = buffers.l_triangle_indices[i];

    auto it1 = buffers.triangle_indices.begin();
    auto it2 = buffers.triangle_indices.begin() + buffers.n_renderable_triangles;

    if (std::find(it1, it2, t_index) != it2) {

      FragmentOperation::l_matrices[t_index] = true;
      const Triangle& t = buffers.light_triangles[t_index];
      UV fakeuv;

      fakeuv.p = t.a;
      fakeuv.u = t.b;
      fakeuv.v = t.c;

      // Normalize UV to the lightmap size
      unsigned lightmap_size = buffers.l_triangle_index_surface.height();
      fakeuv.p.X /= lightmap_size;
      fakeuv.p.Y /= lightmap_size;
      fakeuv.u.X /= lightmap_size;
      fakeuv.u.Y /= lightmap_size;
      fakeuv.v.X /= lightmap_size;
      fakeuv.v.Y /= lightmap_size;

      FragmentOperation::lightness_projectors[t_index].generate_uv_projector(
                                               buffers.triangles[t_index],
                                               fakeuv);
    }
  });
}

void FragmentShader::push_operation(FragmentOperation* op) {
  operations.push_back(op);
}

std::vector<TextureProjector> FragmentOperation::texture_projectors;

std::vector<TextureProjector> FragmentOperation::lightness_projectors;
std::vector<bool> FragmentOperation::l_matrices;

Texture<unsigned char, 3> FragmentOperation::texture;
Texture<unsigned char, 3> FragmentOperation::normal_map;

FragmentOperation::FragmentOperation() :
  buffers (CommonBuffers::get())
{}
