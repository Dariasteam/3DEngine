#include "abstractrasteriserCPU.h"

void AbstractRasteriserCPU::triangle_to_surface_space(Triangle &triangle) const {
  double v_factor = camera->get_bounds().height * 2;
  double h_factor = camera->get_bounds().height * 2;

  unsigned y_offset = std::round(camera->get_bounds().height);
  unsigned x_offset = std::round(camera->get_bounds().width);

  triangle.a.set_x(std::round((triangle.a.x() + x_offset ) * height / v_factor));
  triangle.a.set_y(std::round((triangle.a.y() + y_offset ) * width  / h_factor));

  triangle.b.set_x(std::round((triangle.b.x() + x_offset ) * height / v_factor));
  triangle.b.set_y(std::round((triangle.b.y() + y_offset ) * width  / h_factor));

  triangle.c.set_x(std::round((triangle.c.x() + x_offset ) * height / v_factor));
  triangle.c.set_y(std::round((triangle.c.y() + y_offset ) * width  / h_factor));
}


void AbstractRasteriserCPU::rasterise(const Camera& cam,
                                      Texture<unsigned long, 1>& i_surface,
                                      Texture<double, 1>& z_surface) {
  camera = &cam;
  indices_target_surface = &i_surface;
  z_target_surface = &z_surface;

  height = indices_target_surface->height();
  width = indices_target_surface->width();

  z_target_surface->fill(INFINITY_DISTANCE);

  auto& m = MultithreadManager::get_instance();  

  m.calculate_threaded(buffers.n_renderable_triangles, [&](unsigned i) {
    unsigned t_index = buffers.triangle_indices[i];
    rasterize_triangle(buffers.triangles[t_index], t_index);
  });    
}
