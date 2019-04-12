#ifndef CANVAS_H
#define CANVAS_H

#include <gtkmm/drawingarea.h>

#include <vector>
#include <algorithm>

#include "../../../engine/point2d.h"

class Canvas : public Gtk::DrawingArea {
private:
  double x_offset; // adjust x = 0 point to the center of screen
  double y_offset; // adjust y = 0 point to the center of screen

  double v_factor;
  double h_factor;
  
  std::vector<Triangle2> triangles;
	
  inline Point2 adjust_coordinates (const Point2& p);
  inline void repaint();
public:
  Canvas();
  virtual ~Canvas();
  void update_frame (const std::vector<Triangle2>& elements, Rect bounds);
protected:
  //Override default signal handler:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  void on_size_allocate(Gtk::Allocation& allocation) override;
};

#endif // CANVAS_H
