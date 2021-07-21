#include "shadowlesslightning.h"

void ShadowlessLightning::operator ()(unsigned pixel_index) {

  Color base_color {
    double(buffers.screen_buffer.get(pixel_index * 3 +  0)),
    double(buffers.screen_buffer.get(pixel_index * 3 +  1)),
    double(buffers.screen_buffer.get(pixel_index * 3 +  2)),
  };

  double minimum_light_contribution = 0;
  Vector3 light_contribution = {minimum_light_contribution,
                                minimum_light_contribution,
                                minimum_light_contribution};

  // Calculate incidence
  const DirectionalLight& light = World::get().get_light();

  // Calculate normal maps
  Vector2 normalVec {
    double(buffers.normal_buffer.get(pixel_index * 3 + 0) - 128) / 127,
    double(buffers.normal_buffer.get(pixel_index * 3 + 1) - 128) / 127
  };

  Vector2 lightVec = {
    light.get_global_plane_vector().toVector3().X,
    light.get_global_plane_vector().toVector3().Y
  };

  double incidence = lightVec * normalVec;
  double intensity = light.get_intensity();

  Color light_color = light.get_color();

  buffers.screen_buffer.set (pixel_index * 3 + 0, clamp(base_color.X * incidence * light_color.X * intensity));
  buffers.screen_buffer.set (pixel_index * 3 + 1, clamp(base_color.Y * incidence * light_color.Y * intensity));
  buffers.screen_buffer.set (pixel_index * 3 + 2, clamp(base_color.Z * incidence * light_color.Z * intensity));
}
