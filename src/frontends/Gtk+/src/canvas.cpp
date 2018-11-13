#include "canvas.h"
#include <cairomm/context.h>

Canvas::Canvas()
{
}

Canvas::~Canvas()
{
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {  

  for (const auto& triangle: triangles) {
    const Point2 a = adjust_coordinates(triangle.a);
    const Point2 b = adjust_coordinates(triangle.b);
    const Point2 c = adjust_coordinates(triangle.c);

		cr->set_line_width(0.0);  	

		cr->set_source_rgb(
			(double)triangle.color.r / 255,
      (double)triangle.color.g / 255,
      (double)triangle.color.b / 255		
		);    
		
		cr->move_to (a.x(), a.y());
		cr->line_to (b.x(), b.y());
		cr->line_to (c.x(), c.y());		
		cr->close_path ();
        
    cr->stroke_preserve();
    cr->fill();
  }
  return true;
}

void Canvas::update_frame(const std::vector<Triangle2>& elements, Rect b) {
  triangles = elements;

  std::sort (triangles.begin(), triangles.end(),
             [](const Triangle2& a, const Triangle2& b) {
      return a.z_value > b.z_value;
    });
	
	const int width = get_allocation().get_width();
	const int height = get_allocation().get_height();

  v_factor = height / b.size_y();
  h_factor = width  / b.size_x();

  repaint();
}

// Translates the coordinates to the canvas size and
// the non centered coordinate system

Point2 Canvas::adjust_coordinates(const Point2& p) {
  return {(p.x() * h_factor + x_offset),
          (p.y() * v_factor + y_offset)
         };
}

void Canvas::on_size_allocate(Gtk::Allocation& allocation) {
	x_offset = allocation.get_width() / 2;
  y_offset = allocation.get_height() / 2;
  set_allocation(allocation);
}

void Canvas::repaint() {
	auto win = get_window();
  if (win) {
    Gdk::Rectangle r(0, 0, get_allocation().get_width(), 
    		get_allocation().get_height());
    win->invalidate_rect(r, true);
  }
}