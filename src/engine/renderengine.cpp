#include "renderengine.h"

RenderEngine::RenderEngine() :
    world(World::get()),
    fps_render("RENDER")
  {
  CommonBuffers::get().set_dimension(1000, 1000);

  //fragmentShader.push_operation(new FlatNormals());
  fragmentShader.push_operation(new SmoothNormals());
  //fragmentShader.push_operation(new NormalMapping());
  fragmentShader.push_operation(new TexturePainter());
  //fragmentShader.push_operation(new ShadowlessLightning());
  fragmentShader.push_operation(new Lightness());
}

void RenderEngine::render_loop () {
  auto& buffers = CommonBuffers::get();
  canvas.target = &buffers.screen_buffer;

  world.get_light().set_triangle_buffer(&buffers.light_triangles);
  world.get_camera().set_triangle_buffer(&buffers.triangles);

  world.get_light().set_n_triangle_buffer(&buffers.n_l_renderable_triangles);
  world.get_camera().set_n_triangle_buffer(&buffers.n_renderable_triangles);

  world.get_light().set_t_indices(&buffers.l_triangle_indices);
  world.get_camera().set_t_indices(&buffers.triangle_indices);

  while (1) {
    world.calculate_next_frame();

    // Occlude all triangle
    buffers.is_triangle_occluded.set();
    projector.project_camera(world.get_light());

    // Set as unoccluded the triangles visible by the directional light
    for (unsigned i = 0; i < buffers.n_l_renderable_triangles; i++) {
      unsigned long triangle_index = buffers.l_triangle_indices[i];
      buffers.is_triangle_occluded.set(triangle_index, 0);
    }

    rasteriser.rasterise(world.get_light(),
                         buffers.l_triangle_index_surface,
                         buffers.z_light); // FIXME: use z_buffer instead?

    projector.project_camera(world.get_camera());
    rasteriser.rasterise(world.get_camera(),
                         buffers.triangle_index_surface,
                         buffers.z_buffer);

    fps_render.update();

    fragmentShader();
    fps_render.update();


    canvas.paint();
  }
}
