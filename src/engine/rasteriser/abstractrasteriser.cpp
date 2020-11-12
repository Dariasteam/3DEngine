#include "abstractrasteriser.h"

AbstractRasteriser::AbstractRasteriser(World* w) :
  world (w),
  camera (w->get_camera()),
  buffers(CommonBuffers::get())
{}
