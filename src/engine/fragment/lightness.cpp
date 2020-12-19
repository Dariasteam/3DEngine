#include "lightness.h"

#define BASE_LIGHT 1

void Lightness::operator ()(unsigned pixel_index) {    
  unsigned t_index = get_triangle_index_at_pixel_index(pixel_index);

  buffers.screen_buffer.set(pixel_index * 3 + 0, 0);
  buffers.screen_buffer.set(pixel_index * 3 + 1, 0);
  buffers.screen_buffer.set(pixel_index * 3 + 2, 0);

  // Check the triangle exist in the lightmapper
  if (!l_matrices[t_index])
    return;

  // 1. Get coordinate of the screen
  Point2i p = pixel_index_to_screen_coordenates(pixel_index);


  // 2. Get the point in the light texture
  Point2i p_l = lightness_projectors[t_index].get_point_on_uv(p.X,
                                                              p.Y,
                                                              buffers.get().
                                                                lightmap_buffer);

// std::cout << p_l.X << " " << p_l.Y << std::endl;

  unsigned long aux_t = buffers.l_triangle_index_buffer.get(p_l.X, p_l.Y);
  auto it1 = buffers.l_triangle_indices.begin();
  auto it2 = buffers.l_triangle_indices.begin() + buffers.n_l_renderable_triangles;

  if (std::find(it1, it2, aux_t) == it2) {
//    std::cout << "ULTA MAL" << " " << aux_t << " does not exist " << std::endl;
  } else {
//    std::cout << "ULTA BIEN" << " " << aux_t << " does exist " << std::endl;
  }
/*
  if (buffers.l_triangle_index_buffer.get(p_l.X, p_l.Y) != 0)
    std::cout << "efectivamente " << p_l.X << " " << p_l.Y << " "
              << buffers.l_triangle_index_buffer.get(p_l.X, p_l.Y) << " "
              << t_index << std::endl;

*/

  // 4. Check triangle index at point in the lightmap equals the triangle at this pixel, if not, is occluded
  if (buffers.l_triangle_index_buffer.get(p_l.X, p_l.Y) != t_index) {
    return;
  }



  const DirectionalLight& light = World::get().get_light();
  auto& triangle = get_triangle_at_pixel_index(pixel_index);

  double incidence = std::max(0.0, (triangle.normal * light.get_global_plane_vector()));
  //Color color = light.get_color();
  //color *= incidence;

  buffers.screen_buffer.set(pixel_index * 3 + 0, std::min(255.0, incidence * 255 * light.get_intensity()));
  buffers.screen_buffer.set(pixel_index * 3 + 1, std::min(255.0, incidence * 255 * light.get_intensity()));
  buffers.screen_buffer.set(pixel_index * 3 + 2, std::min(255.0, incidence * 255 * light.get_intensity()));

/*

  */
}
