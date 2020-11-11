#include "framebufferhandler.h"

FrameBufferHandler::FrameBufferHandler() {
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

  initialized = true;
}

void FrameBufferHandler::set_screen_buffers(const unsigned char* b_aa,
                                            const unsigned char* b_bb) {
  b_a = b_aa;
  b_b = b_bb;
}

bool FrameBufferHandler::paint(const Texture<unsigned char, 3>& frame) {

  if (initialized) {
    for (unsigned y = 0; y < frame.get_height(); y++) {
      for (unsigned x = 0; x < frame.get_width(); x++) {
        location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                   (y+vinfo.yoffset) * finfo.line_length;

        *(fbp + location + 0) = frame.get(x, y, 2);  // B
        *(fbp + location + 1) = frame.get(x, y, 1);  // G
        *(fbp + location + 2) = frame.get(x, y, 0);  // R
      }
    }
  }

  return true;
}
