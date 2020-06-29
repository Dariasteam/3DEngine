#ifndef CANVAS_H
#define CANVAS_H

#include "../../engine/math/point2d.h"
#include "../../engine/planar/rect.h"
#include "../../engine/planar/triangle.h"

#include <QWidget>
#include <QResizeEvent>
#include <QImage>
#include <QPixmap>
#include <QBoxLayout>
#include <QLabel>

#include <vector>
#include <algorithm>
#include <mutex>
#include <future>

#define SCREEN_SIZE 1000

struct ImagePixel;

class Canvas : public QLabel {
  Q_OBJECT
private:
  QImage image;
  bool new_frame_redered {false};

  double x_offset {0}; // adjust x = 0 point to the center of screen
  double y_offset {0}; // adjust y = 0 point to the center of screen

  double v_factor {1};
  double h_factor {1};

  unsigned n_triangles {0};

  const std::vector<Triangle2>* triangles_buffer_a {nullptr};
  const std::vector<Triangle2>* triangles_buffer_b {nullptr};

  const std::vector<std::vector<Color888>>* screen_buffer_a {nullptr};
  const std::vector<std::vector<Color888>>* screen_buffer_b {nullptr};

  std::mutex mtx;
  bool reading_buffer_a = false;

  inline QPointF adjust_coordinates (const Point2& p);
public:
  explicit Canvas(QWidget *parent = nullptr);
  void update_frame (RectF bounds);
  void resizeEvent(QResizeEvent *event) override;

  void set_screen_buffers (const std::vector<std::vector<Color888>>* buff_a,
                           const std::vector<std::vector<Color888>>* buff_b);

  inline bool reading_from_buffer_a () {
    return reading_buffer_a;
  }

  inline void lock_buffer_mutex () {
    mtx.lock();
  }

  inline void unlock_buffer_mutex () {
    mtx.unlock();
  }

  bool paint();
};

#endif // CANVAS_H
