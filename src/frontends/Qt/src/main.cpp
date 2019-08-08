#include "mainwindow.h"
#include <QApplication>

#include "../../../engine/projector.h"
#include "../../../engine/camera.h"
#include "../../../engine/world.h"
#include "../../../engine/rasteriser/rasteriserinterpolatedvertex.h"
#include "../../../engine/rasteriser/rasteriserflat.h"
#include "../../../engine/rasteriser/rasteriserwireframe.h"
#include "src/renderengine.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  Camera camera;
  World world (&camera);
  Projector projector (&camera, &world);
  RasteriserInterpolatedVertex rasteriser (&world, w.get_canvas());
  RenderEngine engine (&projector, &rasteriser, w.get_canvas(), &world);

  return a.exec();
}
