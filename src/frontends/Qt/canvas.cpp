#include "canvas.h"

Canvas::Canvas(QWidget *parent) :
  QWidget(parent),
  triangles (new std::list<Triangle2>())
{
}

void Canvas::paintEvent(QPaintEvent *event) {
  QPainter p(this);

  for (const auto& triangle: *triangles) {

    const QPointF& a = adjust_coordinates(triangle.a);
    const QPointF& b = adjust_coordinates(triangle.b);
    const QPointF& c = adjust_coordinates(triangle.c);

    QPolygonF poly ({a, b, c});
    QBrush brush;

    brush.setColor(QColor (
                     triangle.color.r,
                     triangle.color.g,
                     triangle.color.b
                   ));    

    brush.setStyle(Qt::SolidPattern);
    p.setBrush(brush);

    QPen pen;
    pen.setBrush(brush);
    pen.setStyle(Qt::NoPen);
    p.setPen(pen);
    p.drawPolygon(poly);
  }
}

void Canvas::update_frame(std::list<Triangle2>* elements, Rect b) {

  elements->sort([](const Triangle2& a, const Triangle2& b) {
      return a.z_value > b.z_value;
  });

  delete triangles;
  triangles = elements;

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
