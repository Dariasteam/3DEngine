#include "rasteriser.h"

void Rasteriser::rasterise(std::vector<Triangle2>& triangles) {
  std::vector<std::vector<Color888>>* buff;

  // 1. Select unused buffer
  canvas->lock_buffer_mutex();
  if (canvas->reading_from_buffer_a())
    buff = &screen_buffer_b;
  else
    buff = &screen_buffer_a;

  // 2. Clear buffers
  std::fill(buff->begin(), buff->end(),
            std::vector<Color888>(screen_size, {0,0,0}));

  std::fill(z_buffer.begin(), z_buffer.end(),
            std::vector<double>(screen_size, 100000));

  // 3. Rasterize
  auto& m = MultithreadManager::get_instance();
  m.calculate_threaded(triangles.size(), [&](unsigned i) {
    rasterize_triangle(triangles[i], buff);
  });

  canvas->unlock_buffer_mutex();                 // Acts like Vsync
  canvas->update_frame(camera->get_bounds());
//  write_file(screen_buffer);
}
