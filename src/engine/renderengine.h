#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <chrono>
#include <ctime>
#include <thread>
#include <string>

#include <utility>

#include "math/point3.h"
#include "math/point3.h"
#include "projector.h"
#include "world.h"
#include "camera.h"
#include "rasteriser/abstractrasteriserCPU.h"

#include "fragment/fragmentshader.h"
#include "fragment/flatnormals.h"

#include "../frontends/framebufferhandler.h"

class FPSControl {
private:
  std::string text;

  std::chrono::time_point<std::chrono::system_clock> last_time;
  unsigned frame_counter {0};

public:

  FPSControl (std::string  t) : text (std::move(t)) {
    last_time = std::chrono::system_clock::now();
  }

  inline void update () {
    frame_counter++;

    std::chrono::time_point<std::chrono::system_clock> start;
    start = std::chrono::system_clock::now();

    float elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                                 (start - last_time).count();
    if (elapsed_seconds > 1000.0) {
      double calc = double(frame_counter) / (double(elapsed_seconds) / 1000);
      std::cout << "FPS " << text << ": " << calc << std::endl;
      frame_counter = 0;
      last_time = std::chrono::system_clock::now();
    }
  }
};

class RenderEngine {
private:
  Projector* projector;
  AbstractRasteriser* rasteriser;
  FrameBufferHandler* canvas;
  World* world;

  FragmentShader fragmentShader;

  std::thread t;

  FPSControl fps_render;
  FPSControl fps_painter;

  void render_loop ();
public:
  RenderEngine(Projector* p, AbstractRasteriser* r, FrameBufferHandler* c, World* w);
  void painting_loop ();
};

#endif // RENDERENGINE_H
