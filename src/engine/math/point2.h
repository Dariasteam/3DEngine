#ifndef POINT2_H
#define POINT2_H

#include "../planar/color.h"

template<typename T>
struct P2 {
  T X;
  T Y;

  Color color = {0,0,0};

  P2 () : P2 (0, 0) {}
  P2 (T x, T y) :
    X(x),
    Y(y)
  {}

  inline T x() const { return X;}
  inline T y() const { return Y;}

  inline void set_x (T v) { X = v; }
  inline void set_y (T v) { Y = v; }
  inline void set_values (T vx, T vy) {
    X = vx;
    Y = vy;
  }

  inline P2<T> operator- (const P2<T>& p) const {
    return {x() - p.x(), y() - p.y()};
  }

  template<typename M>
  inline P2<T> operator+ (const P2<M>& p) const {
    return {x() + p.x(), y() + p.y()};
  }

  template<typename M>
  inline double operator* (const P2<M>& p) const {
    return x() * p.x() + y() * p.y();
  }

  inline double operator/ (const P2<T>& p) const {
    return x() / p.x() + y() / p.y();
  }

  inline P2<T> operator* (const double d) const {
    return {x() * d, y() * d};
  }

  inline P2 operator/ (const double d) const {
    return {x() / d, y() / d};
  }

  inline void operator+= (const P2<T>& p)  {
    set_x(x() + p.x());
    set_y(y() + p.y());
  }

  inline void operator-= (const P2<T>& p)  {
    set_x(x() - p.x());
    set_y(y() - p.y());
  }

  inline void operator*= (const double d) {
    set_x(x() * d);
    set_y(y() * d);
  }

  inline void operator/= (const double d) {
    set_x(x() / d);
    set_y(y() / d);
  }

  static double  vector_module (const P2<T>& v) {
    return std::sqrt(v.x() * v.x() + v.y() * v.y());
  }
};

using Vector2  = P2<double>;
using Point2   = P2<double>;
using P2i  = P2<int>;
using Vector2i = P2<int>;
using Normal2  = P2<double>;

#endif // POINT2_H
