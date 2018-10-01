#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent) {  
}

void Canvas::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  for (const auto& element : points) {    
    QPoint aux = adjust_coordinates(element);
    p.drawPoint(aux);
    //p.drawEllipse(adjust_coordinates(element), 3, 3);
  }

  //p.drawLine(QPoint(0,0), QPoint(10,10));
}

void Canvas::update_frame(std::vector<Point2> elements, Rect b) {
  points = elements;
  screen_size = b;

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
