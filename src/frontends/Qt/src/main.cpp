#include "mainwindow.h"
#include <QApplication>

#include "../../../engine/rasteriser.h"
#include "../../../engine/camera.h"
#include "../../../engine/world.h"
#include "src/renderengine.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  Camera camera;
  World world (&camera);
  Rasteriser rasteriser (w.get_canvas(), &camera, &world);
  RenderEngine engine (&rasteriser, w.get_canvas(), &world);

  return a.exec();
}
