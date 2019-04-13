#include "renderengine.h"

RenderEngine::RenderEngine(Rasteriser *r, Camera *cm, World *w) :
    rasteriser (r),
    camera (cm),
    world (w)
  {
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(main_loop()));    
    timer->start(17);
    last_time = std::chrono::system_clock::now();
}

void RenderEngine::main_loop () {
  rasteriser->rasterize();  
  frame_counter++;
  world->move_right();

  std::chrono::time_point<std::chrono::system_clock> start;
  start = std::chrono::system_clock::now();

  float elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                               (start - last_time).count();
  if (elapsed_seconds > 1000) {
    std::cout << "FPS: " << double(frame_counter) / (double(elapsed_seconds) / 1000) << std::endl;
    frame_counter = 0;
    last_time = std::chrono::system_clock::now();
  }

}

