#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "point3d.h"

class ObjParser {
public:
  ObjParser();
  Mesh* operator () (std::string file_name);
};

#endif // OBJPARSER_H
