#ifndef VERTEX_H
#define VERTEX_H

struct Point2i {
  int X;
  int Y;

  Color color {0,0,0};

  Point2i () : Point2i (0, 0, {0,0,0}) {}
  Point2i (int x, int y, const Color& c) :
    X(x),
    Y(y),
    color (c)
  {}
  Point2i (int x, int y) :
    X(x),
    Y(y)
  {}
  Point2i (const Point2& p) :
    X (static_cast<int>(std::round(p.x()))),
    Y (static_cast<int>(std::round(p.y())))
//    color (c)
  {}

  inline int x() const { return X;}
  inline int y() const { return Y;}

  inline void set_x (int v) { X = v; }
  inline void set_y (int v) { Y = v; }
  inline void set_values (int vx, int vy) {
    X = vx;
    Y = vy;
  }

  inline Point2i operator- (const Point2i& p) const {
    return {x() - p.x(), y() - p.y()};
  }

  inline Point2i operator+ (const Point2i& p) const {
    return {x() + p.x(), y() + p.y()};
  }

  inline double operator* (const Point2i& p) const {
    return x() * p.x() + y() * p.y();
  }

  inline double operator/ (const Point2i& p) const {
    return x() * p.x() + y() * p.y();
  }

  inline Point2i operator* (const int d) const {
    return {x() * d, y() * d};
  }

  inline Point2i operator/ (const int d) const {
    return {x() / d, y() / d};
  }

  inline void operator+= (const Point2i& p)  {
    set_x(x() + p.x());
    set_y(y() + p.y());
  }

  inline void operator-= (const Point2i& p)  {
    set_x(x() - p.x());
    set_y(y() - p.y());
  }

  inline void operator*= (const int d) {
    set_x(x() * d);
    set_y(y() * d);
  }

  inline void operator/= (const int d) {
    set_x(x() / d);
    set_y(y() / d);
  }
};

#endif // VERTEX_H
