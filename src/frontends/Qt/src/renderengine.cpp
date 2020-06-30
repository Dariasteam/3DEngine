#include "renderengine.h"


RenderEngine::RenderEngine(Projector* p, AbstractRasteriser* r, Canvas* c, World* w) :
    projector (p),
    rasteriser (r),
    canvas (c),
    world (w),
    fps_render("RENDER"),
    fps_painter("PAINTER")
  {

  std::thread t1 (&RenderEngine::render_loop, this);
  std::thread t2 (&RenderEngine::painting_loop, this);
  t1.detach();
  t2.detach();


  //auto timer = new QTimer(this);
  //connect(timer, &QTimer::timeout, this, &RenderEngine::painting_loop);
//  std::this_thread::sleep_for (std::chrono::duration<double, std::milli>(120));
  //timer->start(2000);  
}

bool a = false;
std::mutex m;
std::condition_variable cv;

void RenderEngine::painting_loop() {  
  while (1) {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{return a;});
    a = false;

    canvas->paint();
    fps_painter.update();
  }
}

void RenderEngine::render_loop () {  
  while (true) {
/*
    std::sort(triangles.begin(), triangles.end(), [](const Triangle2i& t1, const Triangle2i& t2){
      return t1.z_value < t2.z_value;
    });
*/

  //for (unsigned i = 0; i < 1000000000; i++) {
    world->calculate_next_frame();
    projector->project();

    auto triangles = projector->getElementsToRender();
    auto sz = projector->getNElementsToRender();

    rasteriser->rasterise(triangles, sz);

    fps_render.update();
    a = true;
    cv.notify_one();
  //}
    //painting_loop ();
  }
}
