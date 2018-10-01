#include "renderengine.h"

RenderEngine::RenderEngine(Rasteriser *r, Camera *cm, World *w) :
    rasteriser (r),
    camera (cm),
    world (w)
  {
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(main_loop()));
    timer->start(1);
}

void RenderEngine::main_loop () {
  rasteriser->rasterize();  
  world->move_right();
}

