#include "abstractrasteriserCPU.h"

void AbstractRasteriserCPU::rasterise(std::vector<Triangle2i>& triangles) {
  std::vector<std::vector<Color888>>* buff;
  canvas->update_frame(camera->get_bounds());
  return;
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

  int t_size = triangles.size();
  int offset = (t_size / N_THREADS);

  m.calculate_threaded(triangles.size(), [&](unsigned i) {
    //FIXME: This is because we are pre ordering triangles
    // in base of z value
    int processing_block = floor(double(i) / offset);
    int iteration = i - (processing_block * offset);
    int pos = iteration * N_THREADS + processing_block;
    rasterize_triangle(triangles[pos], buff);
  });

  canvas->unlock_buffer_mutex();                 // Acts like Vsync
  canvas->update_frame(camera->get_bounds());
}
