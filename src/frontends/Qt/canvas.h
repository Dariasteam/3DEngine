#ifndef CANVAS_H
#define CANVAS_H

#include "../../engine/point2d.h"

#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QPolygonF>

#include <vector>
#include <algorithm>
#include <mutex>

class Canvas : public QWidget {
  Q_OBJECT
private:
  double x_offset; // adjust x = 0 point to the center of screen
  double y_offset; // adjust y = 0 point to the center of screen

  double v_factor;
  double h_factor;

  unsigned n_triangles = 0;

  std::vector<Triangle2> triangles_a;
  std::vector<Triangle2> triangles_b;

  std::mutex mtx;
  bool t_a = false;

  inline QPointF adjust_coordinates (const Point2& p);
public:
  explicit Canvas(QWidget *parent = nullptr);
  void paintEvent(QPaintEvent *event);  
  void update_frame (Rect bounds, const std::vector<Triangle2>& aux_tr);
  void resizeEvent(QResizeEvent *event);
  void set_triangles_buffer (const std::vector<Triangle2>* buff);
signals:

public slots:
};

#endif // CANVAS_H
