#ifndef CANVAS_H
#define CANVAS_H

#include "../../engine/point2d.h"

#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QPolygonF>

#include <vector>
#include <algorithm>

class Canvas : public QWidget {
  Q_OBJECT
private:
  double x_offset; // adjust x = 0 point to the center of screen
  double y_offset; // adjust y = 0 point to the center of screen

  double v_factor;
  double h_factor;


  std::list<Triangle2>* triangles;

  inline QPointF adjust_coordinates (const Point2& p);
public:
  explicit Canvas(QWidget *parent = nullptr);
  void paintEvent(QPaintEvent *event);  
  void update_frame (std::list<Triangle2>* elements, Rect bounds);
  void resizeEvent(QResizeEvent *event);
signals:

public slots:
};

#endif // CANVAS_H
