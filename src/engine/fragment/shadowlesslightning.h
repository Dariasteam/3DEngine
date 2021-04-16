#ifndef LIGHTNESS_SHADOWLESS_H
#define LIGHTNESS_SHADOWLESS_H

#include "fragmentshader.h"

class ShadowlessLightning: public FragmentOperation {
public:
  void operator () (unsigned pixel_index);

  inline void clamp_color (Color& color) const {
    color.set_x(std::max (0.0, std::min(color.x(), 255.0)));
    color.set_y(std::max (0.0, std::min(color.y(), 255.0)));
    color.set_z(std::max (0.0, std::min(color.z(), 255.0)));
  }

};

#endif // LIGHTNESS_SHADOWLESS_H
