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


void RenderEngine::render_loop () {
  while (1) {
    world.calculate_next_frame();

    auto& buffers = CommonBuffers::get();

    unsigned screen_sz = buffers.get_height();


    projector.project_camera(world.get_light());
    rasteriser.rasterise(world.get_light(),
                         buffers.l_triangle_index_buffer,
                         buffers.z_buffer);

    // Reduce triangle_indices so l_triangle_indices only contains indices of visible triangles
    // (those which have passed de z_buffer test)


    buffers.l_triangle_indices.clear();
    std::unordered_set <unsigned long> light_indices;

    for (unsigned i = 0; i < screen_sz; i++) {
      for (unsigned j = 0; j < screen_sz; j++) {
        if (buffers.z_buffer.get(i, j) < INFINITY_DISTANCE) {
          light_indices.insert(buffers.l_triangle_index_buffer.get(i, j));
        }
      }
    }

    for (const unsigned long& triangle_index : light_indices) {
      buffers.light_triangles[triangle_index] = buffers.triangles[triangle_index];
      buffers.l_triangle_indices.push_back(triangle_index);
    }

    buffers.n_l_renderable_triangles = light_indices.size();

    projector.project_camera(world.get_camera());
    rasteriser.rasterise(world.get_camera(),
                         buffers.triangle_index_buffer,
                         buffers.z_buffer);


    fragmentShader();
    fps_render.update();

    canvas.paint(buffers.screen_buffer);
  }
}
