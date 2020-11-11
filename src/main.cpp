#include "engine/projector.h"
#include "engine/spatial/camera.h"
#include "engine/world.h"
#include "engine/rasteriser/rasteriserflat.h"
#include "engine/renderengine.h"
#include "frontends/framebufferhandler.h"

#include "engine/planar/textureprojector.h"

int main(int argc, char *argv[]) {
  FrameBufferHandler framebuffer;
  PerspectiveCamera camera;
  World world (&camera);
  Projector projector (&camera, &world);
  RasteriserFlat rasteriser(&world);
  RenderEngine engine (&projector, &rasteriser, &framebuffer, &world);
}
