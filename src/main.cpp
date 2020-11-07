#include "engine/projector.h"
#include "engine/camera.h"
#include "engine/world.h"
//#include "engine/rasteriser/rasteriserinterpolatedvertex.h"
//#include "engine/rasteriser/rasterizetextured.h"
//#include "../../../engine/rasteriser/rasteriseropencl.h"
#include "engine/rasteriser/rasteriserflat.h"
//#include "../../../engine/rasteriser/rasteriserwireframe.h"
#include "engine/renderengine.h"
#include "frontends/framebufferhandler.h"

#include "engine/planar/textureprojector.h"

int main(int argc, char *argv[]) {
  //QApplication a(argc, argv);
  //MainWindow w;
  //w.show();

  FrameBufferHandler framebuffer;
  Camera camera;
  World world (&camera);
  Projector projector (&camera, &world);

  //RasterizeTextured rasteriser (&world);
  //RasteriserInterpolatedVertex rasteriser (&world, &framebuffer);

  RasteriserFlat rasteriser(&world);
  RenderEngine engine (&projector, &rasteriser, &framebuffer, &world);

  /*
  Texture tex;
  tex.load ("pic.ppm");

  TextureProjector p;


  Triangle2i triangle ({0,  0},
                       {0, 300},
                       {600, 300},
             100, Color());

  UV uv {
    {0.0, 0.0}, // Point

    {0.0, .5}, // Vec U
    {1, .5}  // Vec V
  };

  p.project(tex,
            triangle,
            uv);
  */

  //tex.write("pic_2.ppm");
  //return a.exec();
}
