#include "abstractrasteriserCPU.h"

void AbstractRasteriserCPU::triangle_to_screen_space (Triangle& triangle) const {

  double v_factor = buffers.get_height() / camera->get_bounds().y;
  double h_factor = buffers.get_width()  / camera->get_bounds().x;

  unsigned y_offset = buffers.get_height() / 2;
  unsigned x_offset = buffers.get_width()  / 2;

  triangle.a.set_x(std::round(triangle.a.x() * h_factor + x_offset));
  triangle.a.set_y(std::round(triangle.a.y() * v_factor + y_offset));

  triangle.b.set_x(std::round(triangle.b.x() * h_factor + x_offset));
  triangle.b.set_y(std::round(triangle.b.y() * v_factor + y_offset));

  triangle.c.set_x(std::round(triangle.c.x() * h_factor + x_offset));
  triangle.c.set_y(std::round(triangle.c.y() * v_factor + y_offset));
}

void AbstractRasteriserCPU::rasterise(const Camera& cam) {
  camera = &cam;
  buffers.reset_z_buffer();
  auto& m = MultithreadManager::get_instance();  

  m.calculate_threaded(buffers.n_renderable_triangles, [&](unsigned i) {
    unsigned t_index = buffers.triangle_indices[i];
    rasterize_triangle(buffers.triangles[t_index], t_index);
  });    
}
