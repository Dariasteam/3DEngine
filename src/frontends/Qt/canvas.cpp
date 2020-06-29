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

void Canvas::set_screen_buffers(const std::vector<std::vector<Color888>>* buff_a,
                                const std::vector<std::vector<Color888>>* buff_b,
                                const unsigned char* b_aa,
                                const unsigned char* b_bb) {
  screen_buffer_a = buff_a;
  screen_buffer_b = buff_b;

  b_a = b_aa;
  b_b = b_bb;
}

bool Canvas::paint() {  
  if (!new_frame_redered)
    return false;

  new_frame_redered = false;
  unsigned screen_size = SCREEN_SIZE;
  const std::vector<std::vector<Color888>>* screen_buffer;
  const unsigned char* buffer_;

  // Select unused buffer and mark it as used
  lock_buffer_mutex();
  if (reading_from_buffer_a()) {
    screen_buffer = screen_buffer_b;
    buffer_ = b_a;
  } else {
    screen_buffer = screen_buffer_a;
    buffer_ = b_b;
  }
  reading_buffer_a = !reading_buffer_a;

  //buffer_ = new unsigned char[3 * screen_size * screen_size];
/*
  // FIXME: This copy does not make sense
  auto lambda = [&](unsigned i) {
    for (unsigned j = 0; j < screen_size; j++) {
      const Color888& aux_color = (*screen_buffer)[j][i];

      buffer_[3 * (j * screen_size + i)    ] = aux_color.r;
      buffer_[3 * (j * screen_size + i) + 1] = aux_color.g;
      buffer_[3 * (j * screen_size + i) + 2] = aux_color.b;
    }
  };

  auto& m = MultithreadManager::get_instance();  
  m.calculate_threaded(1000, lambda);
*/

  QImage image(buffer_, screen_size, screen_size, QImage::Format_RGB888);
  unlock_buffer_mutex();

  setPixmap(QPixmap::fromImage(image));

  //delete[] buffer_;
  return true;
}

