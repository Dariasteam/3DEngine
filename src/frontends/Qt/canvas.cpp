#include "canvas.h"

#define FILL

Canvas::Canvas(QWidget *parent) :
  QWidget(parent) {
}

void Canvas::paintEvent(QPaintEvent *event) {
  QPainter p(this);

  for (const auto triangle: *triangles) {
    const QPointF a = adjust_coordinates(triangle->a);
    const QPointF b = adjust_coordinates(triangle->b);
    const QPointF c = adjust_coordinates(triangle->c);

    QPointF points[3] = {a, b, c};
    #ifdef FILL
      p.setPen(Qt::NoPen);
      p.setBrush(QColor(
        triangle->color.x(),
        triangle->color.y(),
        triangle->color.z()
      ));
    #endif
    p.drawConvexPolygon(points, 3);
  }
}

void Canvas::update_frame(Rect b) {
  v_factor = size().height() / b.size_y();
  h_factor = size().width()  / b.size_x();

  repaint();
}

// Translates the coordinates to the canvas size and
// the non centered coordinate system
QPointF Canvas::adjust_coordinates(const Point2& p) {
  return {(p.x() * h_factor + x_offset),
          (p.y() * v_factor + y_offset)
         };
}

void Canvas::resizeEvent(QResizeEvent *event) {
  x_offset = event->size().width()  / 2;
  y_offset = event->size().height() / 2;
}

void Canvas::set_triangles_buffer(const std::vector<Triangle2*>* buff) {
  triangles = buff;
}
