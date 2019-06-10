#include "canvas.h"

//#define WIREFRAME

Canvas::Canvas(QWidget *parent) :
  QWidget(parent) {
}

void Canvas::paintEvent(QPaintEvent *event) {
  QPainter p(this);


  const std::vector<Triangle2>* triangles;

  mtx.lock();
  if (!t_a)
    triangles = &triangles_a;
  else
    triangles = &triangles_b;
  t_a = !t_a;
  mtx.unlock();

  for (const auto& triangle : *triangles) {
    const QPointF a = adjust_coordinates(triangle.a);
    const QPointF b = adjust_coordinates(triangle.b);
    const QPointF c = adjust_coordinates(triangle.c);

    QPointF points[3] = {a, b, c};
    #ifndef WIREFRAME
      p.setPen(Qt::NoPen);
      p.setBrush(QColor(
        triangle.color.x(),
        triangle.color.y(),
        triangle.color.z()
      ));
    #endif
    p.drawConvexPolygon(points, 3);
  }  
}

void Canvas::update_frame(Rect b, const std::vector<Triangle2>& aux_tr) {
  v_factor = size().height() / b.size_y();
  h_factor = size().width()  / b.size_x();

  mtx.lock();
  if (t_a) {
    triangles_b = aux_tr;
  } else {
    triangles_a = aux_tr;
  }
  mtx.unlock();
  //repaint();
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

void Canvas::set_triangles_buffer(const std::vector<Triangle2>* buff) {
  //triangles = buff;
}
