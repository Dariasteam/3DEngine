#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <chrono>
#include <ctime>
#include <string>

#include "math/point3.h"
#include "math/point3.h"
#include "projector.h"
#include "world.h"
#include "spatial/perspectivecamera.h"
#include "rasteriser/abstractrasteriserCPU.h"
#include "rasteriser/rasteriserflat.h"
#include "planar/textureprojector.h"

#include "fragment/fragmentshader.h"
#include "fragment/flatnormals.h"
#include "fragment/smoothnormals.h"
#include "fragment/normalmapping.h"
#include "fragment/texturepainter.h"
#include "fragment/shadowlesslightning.h"
#include "fragment/lightness.h"

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
      std::cout << "\rFPS " << text << ": " << calc << std::flush;
      frame_counter = 0;
      last_time = std::chrono::system_clock::now();
    }
  }
};

class RenderEngine {
private:
  Projector projector;
  RasteriserFlat rasteriser;
  FrameBufferHandler<unsigned char, 3> canvas;

  FragmentShader fragmentShader;
  World& world;

  FPSControl fps_render;
public:
  RenderEngine();
  void render_loop ();
};

#endif // RENDERENGINE_H
