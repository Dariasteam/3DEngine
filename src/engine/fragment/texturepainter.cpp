#include "texturepainter.h"

void TexturePainter::operator ()(unsigned pixel_index) {
  P2i p = pixel_index_to_screen_coordinates(pixel_index);

  unsigned t_index = get_triangle_index_at_pixel_index(pixel_index);

  Color888 c = texture_projectors[t_index].get_color_on_uv(p.X, p.Y, texture);

  buffers.screen_buffer.set(p.X, p.Y, c.r, 0);
  buffers.screen_buffer.set(p.X, p.Y, c.g, 1);
  buffers.screen_buffer.set(p.X, p.Y, c.b, 2);
}
