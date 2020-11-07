#include "framebufferhandler.h"

FrameBufferHandler::FrameBufferHandler() {  
  // Open the file for reading and writing  
  fbfd = open("/dev/fb0", O_RDWR);
  if (fbfd == -1) {
      perror("Error: cannot open framebuffer device");
      return;
      exit(1);
  }
  printf("The framebuffer device was opened successfully.\n");

  // Get fixed screen information
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
      perror("Error reading fixed information");
      return;
      exit(2);
  }

  // Get variable screen information
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
      perror("Error reading variable information");
      return;
      exit(3);
  }

  printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

  // Figure out the size of the screen in bytes
  screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

  // Map the device to memory
  fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
  if ((long int)fbp == -1) {
      perror("Error: failed to map framebuffer device to memory");
      return;
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

/*
bool FrameBufferHandler::paint() {
  const unsigned char* buffer_;

  // Select unused buffer and mark it as used
  lock_buffer_mutex();
  if (reading_from_buffer_a()) {
    buffer_ = b_b;
    reading_buffer_a = false;
  } else {
    buffer_ = b_a;
    reading_buffer_a = true;
  }  
  unlock_buffer_mutex();

  if (initialized) {
    for (unsigned y = 0; y < SCREEN_SIZE; y++) {
      for (unsigned x = 0; x < SCREEN_SIZE; x++) {
        location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                   (y+vinfo.yoffset) * finfo.line_length;

        *(fbp + location + 0) = buffer_[y * SCREEN_SIZE * 3 + x * 3 + 2]; // B
        *(fbp + location + 1) = buffer_[y * SCREEN_SIZE * 3 + x * 3 + 1]; // G
        *(fbp + location + 2) = buffer_[y * SCREEN_SIZE * 3 + x * 3 + 0]; // R
      }
    }
  }

  return true;
}
*/

bool FrameBufferHandler::paint(const Texture<unsigned long, 1>& frame) {

  if (initialized) {
    for (unsigned y = 0; y < frame.get_height(); y++) {
      for (unsigned x = 0; x < frame.get_width(); x++) {
        location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                   (y+vinfo.yoffset) * finfo.line_length;

        *(fbp + location + 0) = frame.get(x, y, 2) * 100; // B
        *(fbp + location + 1) = frame.get(x, y, 1) * 100; // G
        *(fbp + location + 2) = frame.get(x, y, 0) * 50; // R
      }
    }
  }

  return true;
}
