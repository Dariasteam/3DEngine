#include "canvas.h"

//#define WIREFRAME

Canvas::Canvas(QWidget *parent) :
  QLabel(parent),
  image (SCREEN_SIZE, SCREEN_SIZE, QImage::Format_RGB888)
{}

void Canvas::update_frame(RectF b) {
  v_factor = size().height() / b.size_y();
  h_factor = size().width()  / b.size_x();
  new_frame_redered = true;
}

void Canvas::resizeEvent(QResizeEvent *event) {
  x_offset = static_cast<double>(event->size().width())  / 2;
  y_offset = static_cast<double>(event->size().height()) / 2;
}

void Canvas::set_screen_buffers(const unsigned char* b_aa,
                                const unsigned char* b_bb) {
  b_a = b_aa;
  b_b = b_bb;
}

bool Canvas::paint() {  
  if (!new_frame_redered)
    return false;

  new_frame_redered = false;
  unsigned screen_size = SCREEN_SIZE;  
  const unsigned char* buffer_;

  // Select unused buffer and mark it as used
  lock_buffer_mutex();
  if (reading_from_buffer_a()) {
    buffer_ = b_a;
    reading_buffer_a = false;
  } else {
    buffer_ = b_b;
    reading_buffer_a = true;
  }
  unlock_buffer_mutex();

  QImage image(buffer_, screen_size, screen_size, QImage::Format_RGB888);
  setPixmap(QPixmap::fromImage(image));

  return true;
}

