#include "renderengine.h"


RenderEngine::RenderEngine(Rasteriser *r, Canvas* c, World* w) :
    rasteriser (r),
    canvas (c),
    world (w),
    fps_render("RENDER"),
    fps_painter("PAINTER")
  {

  std::unique_lock<std::mutex> lk(m);

  std::thread t (&RenderEngine::render_loop, this);
  t.detach();

  auto painter_timer = new QTimer(this);
  connect(painter_timer, &QTimer::timeout, this, &RenderEngine::painting_loop);
  painter_timer->start(17);

  auto render_timer = new QTimer(this);
  connect(render_timer, &QTimer::timeout, this, &RenderEngine::render_frame);
  render_timer->start(17.1);
}

void RenderEngine::painting_loop() {
  canvas->repaint();
  fps_painter.update();
}

void RenderEngine::render_frame() {
  ready = true;
  cv.notify_one();
}

void RenderEngine::render_loop () {
  while (true) {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [&]{return ready;});
    ready = false;
    world->calculate_next_frame();
    rasteriser->rasterise();
    fps_render.update();
  }
}
