#include "texturepainter.h"

void TexturePainter::operator ()(unsigned pixel_index) const {
  if (CommonBuffers::get().z_buffer.get(pixel_index) == INFINITY) return;
}
