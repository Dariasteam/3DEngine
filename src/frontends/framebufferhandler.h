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

// FIXME Use a texture alias as template
template <typename T,unsigned D>
class FrameBufferHandler {
private:
  std::condition_variable cv[N_THREADS];
  bool painters [N_THREADS];

  std::condition_variable cv_2;

  int fbfd = 0;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  long int screensize = 0;

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
    for (unsigned i = 0; i < N_THREADS; i++)
      munmap(fup[i], screensize);

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

    double segment = 1000.0 / N_THREADS;

    for (unsigned k = 0; k < N_THREADS; k++) {
      painters[k] = false;

      threads[k] = new std::thread([&, k, segment]() -> void {

        fup[k] = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

        while (1) {
          std::mutex mtx;
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
                  *(fup[k] + location + m_d - i) = a;
                }

              } else {

                for (int i = 0; i < 3; i++) {
                  auto a = (target->get(x, y, i));

                  if (a < INFINITY_DISTANCE) {
                    const double slope_parameter = 0.1;
                    a = -1/(slope_parameter * a + 0.2) + 1;
                    a = 1 - a;
                    a *= 255;
                  } else {
                    a = 0;
                  }
                  *(fup[k] + location + i) = static_cast<unsigned char>(a);
                }
              }
            }
          }
          painters[k] = false;
          cv_2.notify_one();
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

    // Prevent asynchronous painting
    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx); // wake up thread
    cv_2.wait(lck, [&] {
      for (unsigned i = 0; i < N_THREADS; i++) {
        if (painters[i]) return false;
      }
      return true;
    });
  }
};

#endif // FRAMEBUFFERHANDLER_H
