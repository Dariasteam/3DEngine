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

  const std::vector<Triangle2>* triangles_buffer_a;
  const std::vector<Triangle2>* triangles_buffer_b;

  std::mutex mtx;
  bool reading_buffer_a = false;

  inline QPointF adjust_coordinates (const Point2& p);
public:
  explicit Canvas(QWidget *parent = nullptr);
  void paintEvent(QPaintEvent *event);  
  void update_frame (Rect bounds);
  void resizeEvent(QResizeEvent *event);
  void set_triangles_buffer (const std::vector<Triangle2>* buff_a,
                             const std::vector<Triangle2>* buff_b);

  inline bool reading_from_buffer_a () {
    return reading_buffer_a;
  }

  inline void lock_buffer_mutex () {
    mtx.lock();
  }

  inline void unlock_buffer_mutex () {
    mtx.unlock();
  }

signals:

public slots:
};

#endif // CANVAS_H
