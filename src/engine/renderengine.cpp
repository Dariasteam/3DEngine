#include "renderengine.h"


RenderEngine::RenderEngine(Projector* p, AbstractRasteriser* r, FrameBufferHandler* c, World* w) :
    projector (p),
    rasteriser (r),
    canvas (c),
    world (w),
    fragmentShader(w),
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

  render_loop();
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
    world->calculate_next_frame();
    projector->project();

    auto triangles = projector->getElementsToRender();    
    auto sz = projector->getNElementsToRender();

    rasteriser->rasterise(triangles, sz);    

    // Execute shading
    fragmentShader(triangles);

    fps_render.update();
    //a = true;

    canvas->paint(CommonBuffers::get().screen_buffer);
    //cv.notify_one();    
  }
}
