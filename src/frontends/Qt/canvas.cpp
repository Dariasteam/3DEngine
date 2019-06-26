#include "canvas.h"

//#define WIREFRAME

Canvas::Canvas(QWidget *parent) :
  QLabel(parent),
  image (SCREEN_SIZE, SCREEN_SIZE, QImage::Format_RGB888)
{}
/*
void Canvas::paintEvent(QPaintEvent *event) {
  QPainter p(this);

  const std::vector<Triangle2>* triangles;

  lock_buffer_mutex();
  if (reading_from_buffer_a())
    triangles = triangles_buffer_b;
  else
    triangles = triangles_buffer_a;
  reading_buffer_a = !reading_buffer_a;
  unlock_buffer_mutex();

  for (const auto& triangle : *triangles) {
    const QPointF a = adjust_coordinates(triangle.a);
    const QPointF b = adjust_coordinates(triangle.b);
    const QPointF c = adjust_coordinates(triangle.c);

    QPointF points[3] = {a, b, c};
    #ifndef WIREFRAME
      p.setPen(Qt::NoPen);
      p.setBrush(QColor(
        static_cast<int>(triangle.color.x()),
        static_cast<int>(triangle.color.y()),
        static_cast<int>(triangle.color.z())
      ));
    #endif
    p.drawConvexPolygon(points, 3);
  }
}
*/
void Canvas::update_frame(Rect b) {
  v_factor = size().height() / b.size_y();
  h_factor = size().width()  / b.size_x();
  new_frame_redered = true;

//  paint();
}

// Translates the coordinates to the canvas size and
// the non centered coordinate system
QPointF Canvas::adjust_coordinates(const Point2& p) {
  return {
          (p.x() * h_factor + x_offset),
          (p.y() * v_factor + y_offset)
         };
}

void Canvas::resizeEvent(QResizeEvent *event) {
  x_offset = static_cast<double>(event->size().width())  / 2;
  y_offset = static_cast<double>(event->size().height()) / 2;
}

void Canvas::set_screen_buffer(const std::vector<std::vector<Color888>>* buff_a,
                               const std::vector<std::vector<Color888>>* buff_b) {
  screen_buffer_a = buff_a;
  screen_buffer_b = buff_b;
}

/*
void Canvas::set_triangles_buffer(const std::vector<Triangle2> *buff_a,
                                  const std::vector<Triangle2> *buff_b) {
  triangles_buffer_a = buff_a;
  triangles_buffer_b = buff_b;
}

void Canvas::set_screen_buffer(const std::vector<std::vector<ImagePixel> >* buff) {
  screen_buffer = buff;
}
*/
bool Canvas::paint() {  
  if (!new_frame_redered)
    return false;

  new_frame_redered = false;
  unsigned screen_size = SCREEN_SIZE;
  const std::vector<std::vector<Color888>>* screen_buffer;

  // Select unused buffer and mark it as used
  lock_buffer_mutex();
  if (reading_from_buffer_a())
    screen_buffer = screen_buffer_b;
  else
    screen_buffer = screen_buffer_a;
  reading_buffer_a = !reading_buffer_a;
  unlock_buffer_mutex();

  unsigned char* buffer_;
  buffer_ = new unsigned char[3 * screen_size * screen_size];

  // FIXME: This copy does not make sense

  auto lambda = [&](unsigned init_x, unsigned end_x) {
    for (unsigned i = init_x; i < end_x; i++) {
      for (unsigned j = 0; j < screen_size; j++) {
        const Color888& aux_color = (*screen_buffer)[j][i];

        buffer_[3 * (j * screen_size + i)    ] = aux_color.r;
        buffer_[3 * (j * screen_size + i) + 1] = aux_color.g;
        buffer_[3 * (j * screen_size + i) + 2] = aux_color.b;
      }
    }
  };

  unsigned segments = (screen_size / N_THREADS);
  std::vector<std::future<void>> promises (N_THREADS);
  for (unsigned i = 0; i < N_THREADS  - 1; i++)
    promises[i] = std::async(lambda, i * segments, (i + 1) * segments);

  promises[N_THREADS - 1] = std::async(lambda, (N_THREADS - 1) * segments,
                                                screen_size);
  for (auto& promise : promises)
    promise.get();

  QImage image(buffer_, screen_size, screen_size, QImage::Format_RGB888);
  setPixmap(QPixmap::fromImage(image));

  delete buffer_;
  return true;
}

