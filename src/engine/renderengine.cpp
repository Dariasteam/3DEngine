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
    projector.project_camera(world.get_camera());
    rasteriser.rasterise(world.get_camera());
    fragmentShader();
    fps_render.update();
    canvas.paint(CommonBuffers::get().screen_buffer);
  }
}
