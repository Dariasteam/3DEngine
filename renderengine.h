#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <thread>
#include <QTimer>

#include "point3d.h"
#include "rasteriser.h"
#include "world.h"
#include "camera.h"

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
