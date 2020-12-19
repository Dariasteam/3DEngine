#include "lightness.h"

#define BASE_LIGHT 1

void Lightness::operator ()(unsigned pixel_index) {    
  unsigned t_index = get_triangle_index_at_pixel_index(pixel_index);

  Color base_color {
    double(buffers.screen_buffer.get(pixel_index * 3 +  0)),
    double(buffers.screen_buffer.get(pixel_index * 3 +  1)),
    double(buffers.screen_buffer.get(pixel_index * 3 +  2)),
  };

  // Minimum world light
  Color light_contribution = {0.4, 0.4, 0.4};

  // Calculate incidence
  Point2i p = pixel_index_to_screen_coordenates(pixel_index);
  const DirectionalLight& light = World::get().get_light();

  auto normalColor = texture_projectors[t_index].get_color_on_uv(p.X, p.Y, normal_map);
  Vector3 normalVec = {(double(normalColor.r) / 255) - 1,
                       (double(normalColor.g) / 255) - 1,
                       0};

  double incidence = std::max(0.0, (normalVec * light.get_global_plane_vector()));
  light_contribution.X = light_contribution.X * incidence;
  light_contribution.Y = light_contribution.Y * incidence;
  light_contribution.Z = light_contribution.Z * incidence;

  // Check the triangle exist in the lightmapper
  if (l_matrices[t_index]) {

    // Get the point in the light texture
    Point2i p_l = lightness_projectors[t_index].get_point_on_uv(p.X, p.Y,
                                                                buffers.get().lightmap_buffer);

    // Check the triangle index in the lightmapper equals the one at this pixel
    if (buffers.l_triangle_index_buffer.get(p_l.X, p_l.Y) == t_index) {

      double intensity = light.get_intensity();
      Color light_color = light.get_color();

      light_contribution.X = incidence * light_color.X * intensity;
      light_contribution.Y = incidence * light_color.Y * intensity;
      light_contribution.Z = incidence * light_color.Z * intensity;
    }
  }

  buffers.screen_buffer.set (pixel_index * 3 + 0, std::min(255.0, base_color.X * light_contribution.X));
  buffers.screen_buffer.set (pixel_index * 3 + 1, std::min(255.0, base_color.Y * light_contribution.Y));
  buffers.screen_buffer.set (pixel_index * 3 + 2, std::min(255.0, base_color.Z * light_contribution.Z));

}
