#ifndef FRAMEBUFFERHANDLER_H
#define FRAMEBUFFERHANDLER_H

#include "../engine/planar/texture.h"
#include "../engine/buffers/commonbuffers.h"
#include "../auxiliar/multithreadmanager.h"

#include <vector>
#include <algorithm>
#include <mutex>
#include <future>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define SCREEN_SIZE 1000

template <typename T,unsigned D>
class FrameBufferHandler {
private:
  std::condition_variable cv[N_THREADS];
  bool painters [N_THREADS];

  int fbfd = 0;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  long int screensize = 0;
  char *fbp = 0;  

  char* fup[N_THREADS];


  bool initialized {false};

  double x_offset {0}; // adjust x = 0 point to the center of screen
  double y_offset {0}; // adjust y = 0 point to the center of screen

  double v_factor {1};
  double h_factor {1};

  unsigned n_triangles {0};

  std::thread* threads [N_THREADS];
public:
  Texture<T, D>* target;

  ~FrameBufferHandler() {
    munmap(fbp, screensize);
    close(fbfd);
  }

  FrameBufferHandler() {
    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

    if ((long int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");


    double segment = double(1000 / N_THREADS);

    for (unsigned k = 0; k < N_THREADS; k++) {
      painters[k] = false;


      threads[k] = new std::thread([&, k, segment]() -> void {

        fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);        

        std::mutex mtx;

        while (1) {
         std::unique_lock<std::mutex> lck(mtx); // wake up thread
         cv[k].wait(lck, [&]{return painters[k];});
          painters[k] = false;

          for (unsigned y = std::round(segment * k);
                        y < std::round(segment * (k + 1)); y++) {

            for (unsigned x = 0; x < target->width(); x++) {

              long int  location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                         (y+vinfo.yoffset) * finfo.line_length;

              unsigned m_d = 2;

              if constexpr (D == 3) {
                // B G R Alpha
               for (int i = m_d; i >= 0; i--) {
                  auto a = target->get(x, y, i);
                  *(fbp + location + m_d - i) = a;
                }

              } else {

                for (int i = 0; i < 3; i++) {
                  auto a = (target->get(x, y, i));

                  if (a < INFINITY_DISTANCE) {
                    const double slope_parameter = 0.16;
                    a = -1/(slope_parameter * a - .04) + 1;
                    a = 1 - a;
                    a *= 255;
                  } else {
                    a = 0;
                  }
                  *(fbp + location + i) = static_cast<unsigned char>(a);
                }
              }
            }
          }
        }
      });

      threads[k]->detach();
    }
  }

  void paint() {
    for (unsigned i = 0; i < N_THREADS; i++) {
      painters[i] = true;
      cv[i].notify_one();
    }
  }
};

#endif // FRAMEBUFFERHANDLER_H
