#include "lightness.h"

#define BASE_LIGHT 1

void Lightness::operator ()(unsigned pixel_index) {    

  Point2i p1 = pixel_index_to_screen_coordenates(pixel_index);
  unsigned t_index = get_triangle_index_at_pixel_index(pixel_index);

  buffers.screen_buffer.set(pixel_index * 3 + 0, 0);
  buffers.screen_buffer.set(pixel_index * 3 + 1, 0);
  buffers.screen_buffer.set(pixel_index * 3 + 2, 0);

  Point2i p3 = texture_projectors[t_index].get_point_on_uv(p1.X, p1.Y, texture);

  if (std::find(buffers.l_triangle_indices.begin(),
                buffers.l_triangle_indices.end(), t_index) != buffers.l_triangle_indices.end()) {
/*
    Point2i p2 = lightness_projectors[t_index].
                get_point_on_uv(p3.X, p3.Y, buffers.get().lightmap_buffer);

    // Check the triangle in lightmap at the projected coordinate efectively
    // correpsond to the current triangle
    if (buffers.l_triangle_index_buffer.get(p2.X, p2.Y) !=
        buffers.triangle_index_buffer.get(t_index)) {
      return;
    }
*/
    const DirectionalLight& light = World::get().get_light();
    auto& triangle = get_triangle_at_pixel_index(pixel_index);

    double incidence = std::max(0.0, (triangle.normal * light.get_global_plane_vector()));
    //Color color = light.get_color();
    //color *= incidence;

    buffers.screen_buffer.set(pixel_index * 3 + 0, std::min(255.0, incidence * 255 * light.get_intensity()));
    buffers.screen_buffer.set(pixel_index * 3 + 1, std::min(255.0, incidence * 255 * light.get_intensity()));
    buffers.screen_buffer.set(pixel_index * 3 + 2, std::min(255.0, incidence * 255 * light.get_intensity()));
  }

/*

  */
}
