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

  QTimer *painter_timer = new QTimer(this);
  connect(painter_timer, SIGNAL(timeout()), this, SLOT(painting_loop()));
  painter_timer->start(17);
}

void RenderEngine::painting_loop() {
  canvas->repaint();
  fps_painter.update();
}

void RenderEngine::render_loop () {
  while (1) {
    world->calculate_next_frame();
    rasteriser->rasterise();
    fps_render.update();
  }
}
