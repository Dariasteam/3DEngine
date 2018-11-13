#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <thread>
#include <QTimer>

#include "../../engine/point3d.h"
#include "../../engine/rasteriser.h"
#include "../../engine/world.h"
#include "../../engine/camera.h"

class RenderEngine : public QObject{
  Q_OBJECT
private:
  Rasteriser* rasteriser;
  Camera* camera;
  World* world;
public:
  RenderEngine(Rasteriser* r, Camera* cm, World* w);
public slots:
  void main_loop ();
};

#endif // RENDERENGINE_H
