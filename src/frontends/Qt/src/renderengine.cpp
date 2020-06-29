#include "renderengine.h"


RenderEngine::RenderEngine(Projector* p, AbstractRasteriser* r, Canvas* c, World* w) :
    projector (p),
    rasteriser (r),
    canvas (c),
    world (w),
    fps_render("RENDER"),
    fps_painter("PAINTER")
  {

  std::thread t (&RenderEngine::render_loop, this);
  t.detach();

  //auto timer = new QTimer(this);
  //connect(timer, &QTimer::timeout, this, &RenderEngine::painting_loop);
//  std::this_thread::sleep_for (std::chrono::duration<double, std::milli>(120));
  //timer->start(2000);  
}

void RenderEngine::painting_loop() {
//  canvas->repaint();
  if (canvas->paint())
    fps_painter.update();
}

void RenderEngine::render_loop () {
  //while (true) {
  for (unsigned i = 0; i < 30000; i++) {
    world->calculate_next_frame();
    auto triangles = projector->project();

    std::sort(triangles.begin(), triangles.end(), [](const Triangle2i& t1, const Triangle2i& t2){
      return t1.z_value < t2.z_value;
    });

    rasteriser->rasterise(triangles);

    fps_render.update();
    painting_loop ();

    world->calculate_next_frame();
    triangles = projector->project();
    //rasteriser->rasterise(triangles);

    fps_render.update();

  }
    //painting_loop ();
//    std::this_thread::sleep_for (std::chrono::duration<double, std::milli>(12));
  //}
}
