#include "abstractrasteriserCPU.h"

void AbstractRasteriserCPU::rasterise(std::vector<Triangle2i>& triangles) {  
  unsigned char* buff;
  canvas->update_frame(camera->get_bounds());

  // 1. Select unused buffer
  canvas->lock_buffer_mutex();
  if (canvas->reading_from_buffer_a())
    buff = screen_buff_b;
  else
    buff = screen_buff_a;

  // 2. Clear buffers
  std::fill(z_buff, z_buff + SCREEN_SIZE * SCREEN_SIZE, 1000000000000);
  std::fill(buff, buff + SCREEN_SIZE * SCREEN_SIZE * 3, 0);

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
    rasterize_triangle(triangles[i]);
  });

  canvas->unlock_buffer_mutex();                 // Acts like Vsync
  canvas->update_frame(camera->get_bounds());
}
