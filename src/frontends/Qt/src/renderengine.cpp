#include "renderengine.h"


RenderEngine::RenderEngine(Rasteriser *r, Canvas* c, World* w) :
    rasteriser (r),
    canvas (c),
    world (w),
    fps_render("RENDER"),
    fps_painter("PAINTER")
  {

  std::thread t (&RenderEngine::render_loop, this);
  t.detach();

  auto timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &RenderEngine::painting_loop);
  std::this_thread::sleep_for (std::chrono::duration<double, std::milli>(120));
//  timer->start(17);
}

void RenderEngine::painting_loop() {
//  canvas->repaint();
  canvas->paint();
  fps_painter.update();
}

void RenderEngine::render_loop () {
  while (true) {
    world->calculate_next_frame();
    rasteriser->rasterise();
    fps_render.update();

//    std::this_thread::sleep_for (std::chrono::duration<double, std::milli>(12));
  }
}
