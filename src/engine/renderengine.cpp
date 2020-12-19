#include "renderengine.h"

RenderEngine::RenderEngine() :
    world(World::get()),
    fps_render("RENDER")
  {
  CommonBuffers::get().set_dimension(1000, 1000);

  fragmentShader.push_operation(new FlatNormals());
  fragmentShader.push_operation(new TexturePainter());
  fragmentShader.push_operation(new Lightness());
}

// FIXME:
#include <unordered_set>

void RenderEngine::render_loop () {
  while (1) {
    world.calculate_next_frame();

    unsigned screen_sz = CommonBuffers::get().get_height();
    projector.project_camera(world.get_light());
    rasteriser.rasterise(world.get_light(), screen_sz, screen_sz);

    Texture<double, 1> z = CommonBuffers::get().z_buffer;
    Texture<unsigned char, 3> lightmap (z.get_height(), z.get_width());


    for (int i = 0; i < z.get_height() * z.get_width(); i++) {
      if (z.get(i) < INFINITY_DISTANCE) {
        lightmap.set(i * 3 + 0, 255);
        lightmap.set(i * 3 + 1, 255);
        lightmap.set(i * 3 + 2, 255);
      } else {
        lightmap.set(i * 3 + 0, 0);
        lightmap.set(i * 3 + 1, 0);
        lightmap.set(i * 3 + 2, 0);
      }
    }

    CommonBuffers::get().l_triangle_index_buffer = CommonBuffers::get().triangle_index_buffer;
    CommonBuffers::get().lightmap_buffer = lightmap;
    CommonBuffers::get().light_triangles = CommonBuffers::get().triangles;

    // Reduce triangle_indices so l_triangle_indices only contains indices of visible triangles
    // (those which have passed de z_buffer test)

    CommonBuffers::get().l_triangle_indices.clear();
    std::unordered_set <unsigned long> light_indices;

    for (unsigned i = 0; i < screen_sz; i++) {
      for (unsigned j = 0; j < screen_sz; j++) {
        if (CommonBuffers::get().z_buffer.get(i, j) < INFINITY_DISTANCE) {
          light_indices.insert(CommonBuffers::get().l_triangle_index_buffer.get(i, j));
        }
      }
    }

    for (const unsigned long& triangle_index : light_indices) {
      CommonBuffers::get().l_triangle_indices.push_back(triangle_index);
    }
    CommonBuffers::get().n_l_renderable_triangles = light_indices.size();

    projector.project_camera(world.get_camera());
    rasteriser.rasterise(world.get_camera(), screen_sz, screen_sz);

    fragmentShader();

    fps_render.update();

    canvas.paint(CommonBuffers::get().screen_buffer);
  }
}
