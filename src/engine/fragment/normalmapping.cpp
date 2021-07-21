#include "normalmapping.h"

void NormalMapping::operator ()(unsigned pixel_index) {
  Point2i p = pixel_index_to_screen_coordinates(pixel_index);

  unsigned t_index = get_triangle_index_at_pixel_index(pixel_index);
  Color888 c = texture_projectors[t_index].get_color_on_uv(p.X, p.Y, normal_map);

  unsigned r = buffers.normal_buffer.get(pixel_index * 3 + 0);
  unsigned g = buffers.normal_buffer.get(pixel_index * 3 + 1);

  r += (int)c.r - 127;
  g += (int)c.g - 127;

  clamp(r);
  clamp(g);

  buffers.normal_buffer.set(pixel_index * 3 + 0, (unsigned char)r);
  buffers.normal_buffer.set(pixel_index * 3 + 1, (unsigned char)g);
  buffers.normal_buffer.set(pixel_index * 3 + 2, 255);
}
