#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../engine/math/point3.h"
#include "../engine/math/vector3.h"
#include "../engine/spatial/mesh.h"

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
