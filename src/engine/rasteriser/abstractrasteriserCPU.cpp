#include "abstractrasteriserCPU.h"

void AbstractRasteriserCPU::rasterise(std::vector<Triangle2i>* triangles,
                                      unsigned sz) {  
  // 1. Select unused buffer
  canvas->lock_buffer_mutex();
  if (canvas->reading_from_buffer_a())
    buff = screen_buff_b;
  else
    buff = screen_buff_a;

  canvas->unlock_buffer_mutex();                 // Acts like Vsync

  // 2. Clear buffers
  std::fill(z_buff, z_buff + SCREEN_SIZE * SCREEN_SIZE, 1000000000000);
  std::fill(buff, buff + SCREEN_SIZE * SCREEN_SIZE * 3, 0);

  // 3. Rasterize
  auto& m = MultithreadManager::get_instance();

  int t_size = sz;
  int offset = (t_size / N_THREADS);

  m.calculate_threaded(t_size, [&](unsigned i) {
    rasterize_triangle(triangles->operator[](i));
  });    
}
