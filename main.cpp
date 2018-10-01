#include "mainwindow.h"
#include <QApplication>

#include "rasteriser.h"
#include "camera.h"
#include "renderengine.h"
#include "world.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();  

  World world;
  Camera camera;
  Rasteriser rasteriser (w.get_canvas(), &camera, &world);
  RenderEngine engine (&rasteriser, &camera, &world);

  engine.main_loop();

  return a.exec();
}
