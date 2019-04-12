#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <thread>
#include <glibmm/main.h>

#include "../../../engine/point3d.h"
#include "../../../engine/rasteriser.h"
#include "../../../engine/world.h"
#include "../../../engine/camera.h"

class RenderEngine {  
private:
  Rasteriser* rasteriser;
  Camera* camera;
  World* world;
public:
  RenderEngine(Rasteriser* r, Camera* cm, World* w);
  bool main_loop ();  
};

#endif // RENDERENGINE_H
