#include "renderengine.h"

RenderEngine::RenderEngine(Rasteriser *r, Camera *cm, World *w) :
    rasteriser (r),
    camera (cm),
    world (w)
  {
    Glib::signal_timeout().connect(sigc::mem_fun(*this, 
    	&RenderEngine::main_loop), 17);           
}

bool RenderEngine::main_loop () {
  rasteriser->rasterize();  
  world->move_right();
  return true;
}

