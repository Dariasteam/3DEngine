#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "point3d.h"

class ObjParser {
private:
  std::vector <Point3> vertex_list;

  bool parse_vertex (const std::string& line);
  bool parse_face (const std::string& line, Mesh* aux_mesh);
public:
  ObjParser();
  Mesh* operator () (std::string file_name);
};

#endif // OBJPARSER_H
