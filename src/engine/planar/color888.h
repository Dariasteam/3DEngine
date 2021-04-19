#ifndef COLOR888_H
#define COLOR888_H

struct Color888 {
  unsigned char r;
  unsigned char g;
  unsigned char b;

  bool operator != (const Color888& c) const {
    if (r != c.r || g != c.g || b != c.b)
      return true;
    return false;
  }

  explicit Color888 (const Point3& color) :
    r (static_cast<unsigned char>(color.x())),
    g (static_cast<unsigned char>(color.y())),
    b (static_cast<unsigned char>(color.z()))
  {}

  Color888 (unsigned char R,
            unsigned char G,
            unsigned char B) :
    r (R),
    g (G),
    b (B)
  {}
};

#endif // COLOR888_H
