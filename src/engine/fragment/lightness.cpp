#include "lightness.h"

void Lightness::operator ()(unsigned pixel_index) {    
  unsigned t_index = get_triangle_index_at_pixel_index(pixel_index);

  Color base_color {
    double(buffers.screen_buffer.get(pixel_index * 3 +  0)),
    double(buffers.screen_buffer.get(pixel_index * 3 +  1)),
    double(buffers.screen_buffer.get(pixel_index * 3 +  2)),
  };

  double incidence = 0;

  double minimum_light_contribution = 0;
  Vector3 light_contribution = {minimum_light_contribution,
                                minimum_light_contribution,
                                minimum_light_contribution};

  // Calculate incidence
  Point2i p = pixel_index_to_screen_coordenates(pixel_index);
  const DirectionalLight& light = World::get().get_light();


  // Calculate normal maps
  Vector2 normalVec {
    double(buffers.normal_buffer.get(pixel_index * 3 + 0) - 128) / 127,
    double(buffers.normal_buffer.get(pixel_index * 3 + 1) - 128) / 127
  };


  Vector2 lightVec = {
    light.get_global_plane_vector().X,
    light.get_global_plane_vector().Y
  };

  incidence = lightVec * normalVec;

  // Check the triangle exist in the lightmapper
  if (l_matrices[t_index] && incidence > 0) {
    // Get the point in the light    
    // Check the triangle index in the lightmapper equals the one at this pixel

    Point2i p_l1 = lightness_projectors[t_index].get_point_on_uv(p.X, p.Y,
                                                               buffers.l_triangle_index_surface);

    Point2i p_l2 = lightness_projectors[t_index].get_point_on_uv(p.X + 1, p.Y,
                                                               buffers.l_triangle_index_surface);

    Point2i p_l3 = lightness_projectors[t_index].get_point_on_uv(p.X - 1, p.Y,
                                                               buffers.l_triangle_index_surface);

    Point2i p_l4 = lightness_projectors[t_index].get_point_on_uv(p.X, p.Y + 1,
                                                               buffers.l_triangle_index_surface);

    Point2i p_l5 = lightness_projectors[t_index].get_point_on_uv(p.X, p.Y - 1,
                                                               buffers.l_triangle_index_surface);


    Point2i p_l6 = lightness_projectors[t_index].get_point_on_uv(p.X + 1, p.Y + 1,
                                                               buffers.l_triangle_index_surface);

    Point2i p_l7 = lightness_projectors[t_index].get_point_on_uv(p.X + 1, p.Y - 1,
                                                               buffers.l_triangle_index_surface);

    Point2i p_l8 = lightness_projectors[t_index].get_point_on_uv(p.X - 1, p.Y + 1,
                                                               buffers.l_triangle_index_surface);

    Point2i p_l9 = lightness_projectors[t_index].get_point_on_uv(p.X - 1, p.Y - 1,
                                                               buffers.l_triangle_index_surface);

    std::vector<unsigned> indices;
    indices.push_back(buffers.l_triangle_index_surface.get(p_l1.X, p_l1.Y));
    indices.push_back(buffers.l_triangle_index_surface.get(p_l2.X, p_l2.Y));
    indices.push_back(buffers.l_triangle_index_surface.get(p_l3.X, p_l3.Y));
    indices.push_back(buffers.l_triangle_index_surface.get(p_l4.X, p_l4.Y));
    indices.push_back(buffers.l_triangle_index_surface.get(p_l5.X, p_l5.Y));

    indices.push_back(buffers.l_triangle_index_surface.get(p_l6.X, p_l6.Y));
    indices.push_back(buffers.l_triangle_index_surface.get(p_l7.X, p_l7.Y));
    indices.push_back(buffers.l_triangle_index_surface.get(p_l8.X, p_l8.Y));
    indices.push_back(buffers.l_triangle_index_surface.get(p_l9.X, p_l9.Y));

    if (std::find(indices.begin(), indices.end(), t_index) != indices.end()) {
      double intensity = light.get_intensity();
      Color light_color = light.get_color();

      light_contribution.X = (incidence * light_color.X * intensity);
      light_contribution.Y = (incidence * light_color.Y * intensity);
      light_contribution.Z = (incidence * light_color.Z * intensity);
    }
  }

  buffers.screen_buffer.set (pixel_index * 3 + 0, std::min(255.0, base_color.X * incidence * light_contribution.X));
  buffers.screen_buffer.set (pixel_index * 3 + 1, std::min(255.0, base_color.Y * incidence * light_contribution.Y));
  buffers.screen_buffer.set (pixel_index * 3 + 2, std::min(255.0, base_color.Z * incidence * light_contribution.Z));

}
