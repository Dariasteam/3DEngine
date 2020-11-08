#include "texturepainter.h"

void TexturePainter::operator ()(unsigned pixel_index) {
  Point2i p = pixel_index_to_coordenates(pixel_index);

  unsigned t_index = get_triangle_index_at_pixel_index(pixel_index);
  Color888 c = texture_projectors[t_index].get_color_on_uv(p.X, p.Y);

  CommonBuffers::get().screen_buffer.set(p.X, p.Y, c.r, 0);
  CommonBuffers::get().screen_buffer.set(p.X, p.Y, c.g, 1);
  CommonBuffers::get().screen_buffer.set(p.X, p.Y, c.b, 2);
}
