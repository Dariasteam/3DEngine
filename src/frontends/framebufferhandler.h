#ifndef FRAMEBUFFERHANDLER_H
#define FRAMEBUFFERHANDLER_H

//#include "../engine/math/point2d.h"
//#include "../engine/planar/rect.h"
//#include "../engine/planar/triangle.h"
#include "../engine/planar/texture.h"

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

class FrameBufferHandler {
private:
  int fbfd = 0;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  long int screensize = 0;
  char *fbp = 0;  
  long int location = 0;

  bool initialized {false};

  double x_offset {0}; // adjust x = 0 point to the center of screen
  double y_offset {0}; // adjust y = 0 point to the center of screen

  double v_factor {1};
  double h_factor {1};

  unsigned n_triangles {0};

  const unsigned char* b_a {nullptr};
  const unsigned char* b_b {nullptr};

  std::mutex mtx;
  bool reading_buffer_a = false;
public:
  FrameBufferHandler();
  ~FrameBufferHandler() {
    munmap(fbp, screensize);
    close(fbfd);
  }  
  void set_screen_buffers (const unsigned char* b_a,
                           const unsigned char* b_b);

  inline bool reading_from_buffer_a () {
    return reading_buffer_a;
  }

  inline void lock_buffer_mutex () {
    mtx.lock();
  }

  inline void unlock_buffer_mutex () {
    mtx.unlock();
  }

  template <typename T,unsigned D>
  bool paint(const Texture<T, D>& target) {
    if (initialized) {

      for (unsigned y = 0; y < target.get_height(); y++) {
        for (unsigned x = 0; x < target.get_width(); x++) {
          location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                     (y+vinfo.yoffset) * finfo.line_length;

          unsigned m_d = D - 1;
          for (int i = m_d; i >= 0; i--) {
            *(fbp + location + (m_d - i)) = target.get(x, y, i);  // B
          }
        }
      }
      return true;
    } else {
      return false;
    }
  }
};

#endif // FRAMEBUFFERHANDLER_H
