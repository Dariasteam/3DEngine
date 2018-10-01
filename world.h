#ifndef WORLD_H
#define WORLD_H

#include "point3d.h"

#include <vector>

class World {
private:
  std::vector <Point3> elements;
public:
  World();
  inline const std::vector <Point3>& get_elements () { return elements; }
  void move_right ();
};

#endif // WORLD_H
