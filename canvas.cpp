#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent) {  
}

void Canvas::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  for (const auto& triangle: triangles) {

    //QPoint aux = adjust_coordinates(element);
    //p.drawPoint(aux);
    //p.drawEllipse(adjust_coordinates(element), 2, 2);

    QPoint a = adjust_coordinates(triangle.a);
    QPoint b = adjust_coordinates(triangle.b);
    QPoint c = adjust_coordinates(triangle.c);

    QPolygon poly ({a, b, c});

    p.drawPolygon(poly);
  }

  //p.drawLine(QPoint(0,0), QPoint(10,10));
}

void Canvas::update_frame(const std::vector<Triangle2>& elements, Rect b) {
  triangles = elements;

  v_factor = size().height() / b.size_y();
  h_factor = size().width() / b.size_x();

  repaint();
}

// Translates the coordinates to the canvas size and
// the non centered coordinate system
QPoint Canvas::adjust_coordinates(const Point2 p) {
  return {(p.x * h_factor + x_equivalence),
          (p.y * v_factor + y_equivalence)
         };
}

void Canvas::resizeEvent(QResizeEvent *event) {
  x_equivalence = event->size().width() / 2;
  y_equivalence = event->size().height() / 2;
}
