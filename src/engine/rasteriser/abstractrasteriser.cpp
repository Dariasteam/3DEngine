#include "abstractrasteriser.h"

AbstractRasteriser::AbstractRasteriser(World* w) :
  world (w),
  camera (w->get_camera())
{    
  //cv->set_screen_buffers(screen_buff_a, screen_buff_b);
}
