#include "renderengine.h"

RenderEngine::RenderEngine() :
    world(World::get()),
    fps_render("RENDER")
  {
  CommonBuffers::get().set_dimension(1000, 1000);

  fragmentShader.push_operation(new FlatNormals());
  fragmentShader.push_operation(new SmoothNormals());
  //fragmentShader.push_operation(new NormalMapping());
  fragmentShader.push_operation(new TexturePainter());
//  fragmentShader.push_operation(new ShadowlessLightning());
  fragmentShader.push_operation(new Lightness());
}

void RenderEngine::render_loop () {
  while (1) {
    world.calculate_next_frame();

    auto& buffers = CommonBuffers::get();    


    std::fill(buffers.is_triangle_ocluded.begin(),
              buffers.is_triangle_ocluded.end(),
              true);

    for (unsigned i = 0; i < buffers.n_l_renderable_triangles; i++) {
      unsigned long triangle_index = buffers.l_triangle_indices[i];
      buffers.is_triangle_ocluded[triangle_index] = false;
    }

    projector.project_camera(world.get_light());

    rasteriser.rasterise(world.get_light(),
                         buffers.l_triangle_index_surface,
                         buffers.z_light);
    // Copy light values
    buffers.n_l_renderable_triangles = buffers.n_renderable_triangles;    
    auto& m = MultithreadManager::get_instance();
    m.calculate_threaded(buffers.n_l_renderable_triangles, [&](unsigned i) {
      unsigned t_index = buffers.triangle_indices[i];
      buffers.l_triangle_indices[i] = t_index;
      buffers.light_triangles[t_index] = buffers.triangles[t_index];      
    });

    std::vector<bool> aux_occlusion = buffers.is_triangle_ocluded;

    projector.project_camera(world.get_camera());
    rasteriser.rasterise(world.get_camera(),
                         buffers.triangle_index_surface,
                         buffers.z_buffer);

    buffers.is_triangle_ocluded = aux_occlusion;

    fragmentShader();
    fps_render.update();    

    canvas.paint(buffers.screen_buffer);
  }
}
