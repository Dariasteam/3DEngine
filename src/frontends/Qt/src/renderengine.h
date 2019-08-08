#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <chrono>
#include <ctime>
#include <thread>
#include <string>

#include <QTimer>

#include "../../../engine/point3d.h"
#include "../../../engine/projector.h"
#include "../../../engine/world.h"
#include "../../../engine/camera.h"
#include "../../../engine/rasteriser.h"
#include "../canvas.h"

class FPSControl {
private:
  std::string text;

  std::chrono::time_point<std::chrono::system_clock> last_time;
  unsigned frame_counter {0};

public:

  FPSControl (const std::string& t) : text (t) {}

  inline void update () {
    frame_counter++;

    std::chrono::time_point<std::chrono::system_clock> start;
    start = std::chrono::system_clock::now();

    float elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
                                 (start - last_time).count();
    if (elapsed_seconds > 1000) {
      double calc = double(frame_counter) / (double(elapsed_seconds) / 1000);
      std::cout << "FPS " << text << ": " << calc << std::endl;
      frame_counter = 0;
      last_time = std::chrono::system_clock::now();
    }
  }
};

class RenderEngine : public QObject{
  Q_OBJECT
private:
  Projector* projector;
  Rasteriser* rasteriser;
  Canvas* canvas;
  World* world;

  std::thread t;

  FPSControl fps_render;
  FPSControl fps_painter;

  void render_loop ();
public:
  RenderEngine(Projector* p, Rasteriser* r, Canvas* c, World* w);
public slots:  
  void painting_loop ();
};

#endif // RENDERENGINE_H
