#include "canvas.h"

//#define WIREFRAME

Canvas::Canvas(QWidget *parent) :
  QWidget(parent) {
}

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

void Canvas::update_frame(Rect b) {
  v_factor = size().height() / b.size_y();
  h_factor = size().width()  / b.size_x();
}

// Translates the coordinates to the canvas size and
// the non centered coordinate system
QPointF Canvas::adjust_coordinates(const Point2& p) {
  return {(p.x() * h_factor + x_offset),
          (p.y() * v_factor + y_offset)
         };
}

void Canvas::resizeEvent(QResizeEvent *event) {
  x_offset = static_cast<double>(event->size().width())  / 2;
  y_offset = static_cast<double>(event->size().height()) / 2;
}

void Canvas::set_triangles_buffer(const std::vector<Triangle2> *buff_a,
                                  const std::vector<Triangle2> *buff_b) {
  triangles_buffer_a = buff_a;
  triangles_buffer_b = buff_b;
}

