#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <vector>
#include <QResizeEvent>

#include "point2d.h"

class Canvas : public QWidget {
  Q_OBJECT
private:
  double x_equivalence; // adjust x = 0 point to the center of screen
  double y_equivalence; // adjust y = 0 point to the center of screen

  double v_factor;
  double h_factor;

  Rect screen_size;

  std::vector<Point2> points;

  inline QPoint adjust_coordinates (Point2 p);
public:
  explicit Canvas(QWidget *parent = nullptr);
  void paintEvent(QPaintEvent *event);  
  void update_frame (std::vector<Point2> elements, Rect bounds);
  void resizeEvent(QResizeEvent *event);
signals:

public slots:
};

#endif // CANVAS_H
