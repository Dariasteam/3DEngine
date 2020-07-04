#include "engine/projector.h"
#include "engine/camera.h"
#include "engine/world.h"
#include "engine/rasteriser/rasteriserinterpolatedvertex.h"
//#include "../../../engine/rasteriser/rasteriseropencl.h"
//#include "../../../engine/rasteriser/rasteriserflat.h"
//#include "../../../engine/rasteriser/rasteriserwireframe.h"
#include "engine/renderengine.h"
#include "frontends/framebufferhandler.h"

int main(int argc, char *argv[]) {
  //QApplication a(argc, argv);
  //MainWindow w;
  //w.show();

  FrameBufferHandler framebuffer;
  Camera camera;
  World world (&camera);
  Projector projector (&camera, &world);
  //RasteriserOpencl rasteriser (&world, w.get_canvas());

  RasteriserInterpolatedVertex rasteriser (&world, &framebuffer);

  //RasteriserFlat rasteriser(&world, w.get_canvas());
  RenderEngine engine (&projector, &rasteriser, &framebuffer, &world);


  //return a.exec();
}
