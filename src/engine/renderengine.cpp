#include "renderengine.h"

RenderEngine::RenderEngine() :
    world(World::get()),
    fps_render("RENDER"),
    fps_painter("PAINTER")
  {
/*
  std::thread t2 (&RenderEngine::painting_loop, this);  
  t2.detach();
  */
  CommonBuffers::get().set_dimension(1000, 1000);

  // Add shader steps
  fragmentShader.push_operation(new FlatNormals());
  fragmentShader.push_operation(new TexturePainter());
  fragmentShader.push_operation(new Lightness());  
}

bool a = false;
std::mutex m;
std::condition_variable cv;

void RenderEngine::painting_loop() {  
  /*
  while (1) {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{return a;});
    a = false;

    canvas->paint();
    fps_painter.update();
  }
  */
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
