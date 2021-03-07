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


  // FIXME: Work in screen space, only calculate for visible triangles
  MultithreadManager::get_instance().calculate_threaded(N_THREADS,
                                                        [&](unsigned i) {

    double segment = double(buffers.l_triangle_index_surface.width()) / N_THREADS;

    for (int x = segment * i; x < segment * (i + 1); x++){
      for (int y = 0; y < buffers.l_triangle_index_surface.height(); y++) {

        if (buffers.z_light.get(x, y) != INFINITY_DISTANCE) {

          unsigned t_index = buffers.l_triangle_index_surface.get(x, y);

          if (!FragmentOperation::l_matrices[t_index]) {
            FragmentOperation::l_matrices[t_index] = true;
            unsigned lightmap_size = buffers.l_triangle_index_surface.height();
            const Triangle& t = buffers.light_triangles[t_index];
            UV fakeuv;

            // Normalize UV to the lightmap size
            fakeuv.p = Point2{t.a.x(), t.a.y()} / lightmap_size;
            fakeuv.u = Point2{t.b.x(), t.b.y()} / lightmap_size;
            fakeuv.v = Point2{t.c.x(), t.c.y()} / lightmap_size;

            FragmentOperation::lightness_projectors[t_index].generate_uv_projector(
                                                     buffers.triangles[t_index],
                                                     fakeuv);

          }
        }
      }
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
