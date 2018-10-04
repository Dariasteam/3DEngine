#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent) {  
}

void Canvas::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  for (const auto& triangle: triangles) {

    QPointF a = adjust_coordinates(triangle.a);
    QPointF b = adjust_coordinates(triangle.b);
    QPointF c = adjust_coordinates(triangle.c);

    QPolygonF poly ({a, b, c});

    QBrush brush;
    brush.setColor(Qt::red);
    brush.setStyle(Qt::SolidPattern);
    p.setBrush(brush);
    p.drawConvexPolygon(poly);
    //p.drawPolygon(poly);
  }  
}

void Canvas::update_frame(const std::vector<Triangle2>& elements, Rect b) {
  triangles = elements;

  std::sort (triangles.begin(), triangles.end(),
             [](const Triangle2& a, const Triangle2& b) {
      return a.z_value > b.z_value;
    });

  v_factor = size().height() / b.size_y();
  h_factor = size().width()  / b.size_x();

  repaint();
}

// Translates the coordinates to the canvas size and
// the non centered coordinate system
QPointF Canvas::adjust_coordinates(const Point2 p) {
  return {(p.x() * h_factor + x_offset),
          (p.y() * v_factor + y_offset)
         };
}

void Canvas::resizeEvent(QResizeEvent *event) {
  x_offset = event->size().width() / 2;
  y_offset = event->size().height() / 2;
}
