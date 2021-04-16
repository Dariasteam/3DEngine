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

    if (buffers.is_triangle_ocluded[t_index]) {
      // Get the point in the light
      // Check the triangle index in the lightmapper equals the one at this pixel

      Point2i p_l = lightness_projectors[t_index].get_point_on_uv(p.X, p.Y,
                                                                 buffers.l_triangle_index_surface);

      std::vector<unsigned> sourrounding_t;
      if (p_l.x() > 0 && p_l.x() < buffers.z_light.width()  - 1 &&
          p_l.y() > 0 && p_l.y() < buffers.z_light.height() - 1) {

        // FIXME: Theese parameters as a variable for sampling shadows
        for (int k = -1; k < 2; k++) {
          for (int j = -1; j < 2; j++) {
            sourrounding_t.push_back(buffers.l_triangle_index_surface.get(p_l.x() + k, p_l.y() + j));
          }
        }
      }

      if (std::find(sourrounding_t.begin(), sourrounding_t.end(), t_index) != sourrounding_t.end()) {
        double intensity = light.get_intensity();
        Color light_color = light.get_color();

        light_contribution.X = (incidence * light_color.X * intensity);
        light_contribution.Y = (incidence * light_color.Y * intensity);
        light_contribution.Z = (incidence * light_color.Z * intensity);
      }
    } else {
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
