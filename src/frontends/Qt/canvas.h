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

  const unsigned char* b_a {nullptr};
  const unsigned char* b_b {nullptr};

  std::mutex mtx;
  bool reading_buffer_a = false;

  inline QPointF adjust_coordinates (const Point2& p);
public:
  explicit Canvas(QWidget *parent = nullptr);
  void update_frame (RectF bounds);
  void resizeEvent(QResizeEvent *event) override;

  void set_screen_buffers (const unsigned char* b_a,
                           const unsigned char* b_b);

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
