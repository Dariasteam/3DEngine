#include "rasteriseropencl.h"

void RasteriserOpencl::rasterise(std::vector<Triangle2i>& triangles) {
  /*
  unsigned char* buff;
  canvas->update_frame(camera->get_bounds());

  // 1. Select unused buffer
  canvas->lock_buffer_mutex();
  if (canvas->reading_from_buffer_a())
    buff = screen_buff_b;
  else
    buff = screen_buff_a;

  // 2. Clear buffers
  std::fill(z_buff, z_buff + SCREEN_SIZE * SCREEN_SIZE, 100000);
  std::fill(buff, buff + SCREEN_SIZE * SCREEN_SIZE * 3, 0);


  // 3. Transform data to opencl
  cl::CommandQueue queue(context, device);  

  struct {
    unsigned x = SCREEN_SIZE;
    unsigned y = SCREEN_SIZE;
  } size;

  std::vector <unsigned> TR;
  std::vector <unsigned char> CR;
  std::vector <float> Z;

  unsigned n_triangles = triangles.size();
  for (unsigned i = 0; i < n_triangles; i++) {
    TR.push_back (triangles[i].a.x());
    TR.push_back (triangles[i].a.y());
    TR.push_back (triangles[i].b.x());
    TR.push_back (triangles[i].b.y());
    TR.push_back (triangles[i].c.x());
    TR.push_back (triangles[i].c.y());      

    CR.push_back (triangles[i].a.color.x());
    CR.push_back (triangles[i].a.color.y());
    CR.push_back (triangles[i].a.color.z());

    CR.push_back (triangles[i].b.color.x());
    CR.push_back (triangles[i].b.color.y());
    CR.push_back (triangles[i].b.color.z());

    CR.push_back (triangles[i].c.color.x());
    CR.push_back (triangles[i].c.color.y());
    CR.push_back (triangles[i].c.color.z());

    Z.push_back (triangles[i].z_value);
  }

  // 4. Copy data to opencl buffers
  cl::Buffer buffer_triangles_cords(context, CL_MEM_READ_ONLY, sizeof(unsigned)      * TR.size());
  cl::Buffer buffer_triangles_color(context, CL_MEM_READ_ONLY, sizeof(unsigned char) * CR.size());
  cl::Buffer buffer_triangles_z(context, CL_MEM_READ_ONLY, sizeof(float) * n_triangles);

  queue.enqueueWriteBuffer(buffer_triangles_cords, CL_FALSE, 0, sizeof(unsigned)      * TR.size(), TR.data());
  queue.enqueueWriteBuffer(buffer_triangles_color, CL_FALSE, 0, sizeof(unsigned char) * CR.size(), CR.data());
  queue.enqueueWriteBuffer(buffer_triangles_z, CL_FALSE, 0, sizeof(float) * n_triangles, Z.data());

  // 5. Set kernel arguments
  cl::Buffer buffer_imga(context, CL_MEM_WRITE_ONLY, sizeof(unsigned char) * 1000 * 1000 * 3);
  cl::Buffer buffer_zz(context, CL_MEM_READ_WRITE, sizeof(float) * 1000 * 1000 * 3);

  queue.enqueueFillBuffer<float>(buffer_zz, 10000000, 0, sizeof(float) * 1000 * 1000);

  kernel.setArg(0, buffer_imga);
  kernel.setArg(1, buffer_zz);
  kernel.setArg(2, buffer_triangles_cords);
  kernel.setArg(3, buffer_triangles_color);
  kernel.setArg(4, buffer_triangles_z);
  kernel.setArg(5, size);

  queue.finish();


  // 6. Enqueue kernel
  int error_hdl = queue.enqueueNDRangeKernel(kernel, cl::NullRange,
                                                     cl::NDRange(n_triangles));
  queue.flush();
  queue.finish();  

  if (error_hdl)
    std::cerr << "Error enqueing kernel" << std::endl;

  // 7. Retrieve data

  unsigned char* img = new unsigned char[SCREEN_SIZE * SCREEN_SIZE * 3];
  queue.enqueueReadBuffer(buffer_imga, CL_TRUE, 0, sizeof(unsigned char) *
                                       SCREEN_SIZE * SCREEN_SIZE * 3, img);

  queue.flush();
  queue.finish();

  for (unsigned i = 0; i < SCREEN_SIZE; i++) {
    for (unsigned j = 0; j < SCREEN_SIZE; j++) {
      (*buff)[i][j].r = img[i * SCREEN_SIZE * 3 + j * 3 + 0];
      (*buff)[i][j].g = img[i * SCREEN_SIZE * 3 + j * 3 + 1];
      (*buff)[i][j].b = img[i * SCREEN_SIZE * 3 + j * 3 + 2];
    }
  }

  canvas->unlock_buffer_mutex();                 // Acts like Vsync
  canvas->update_frame(camera->get_bounds());
  //
  */
}

