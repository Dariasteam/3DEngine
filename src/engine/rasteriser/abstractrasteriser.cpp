#include "abstractrasteriser.h"

AbstractRasteriser::AbstractRasteriser(World* w, FrameBufferHandler* cv) :
  world (w),
  camera (w->get_camera()),
  canvas (cv)
{    
  cv->set_screen_buffers(screen_buff_a, screen_buff_b);
}
