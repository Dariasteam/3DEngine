#include "objparser.h"

bool ObjParser::parse_vertex(const std::string &line) {
  double v_x, v_y, v_z;

  try {
//    line >> v_x;
//    line >> v_y;
//    line >> v_z;
    vertex_list.push_back(Point3{v_x, v_y, v_z});
  } catch (...) {

//    delete aux_mesh;
    return false;
  }
}

bool ObjParser::parse_face(const std::string &line, Mesh *aux_mesh) {

}

ObjParser::ObjParser() {

}

Mesh *ObjParser::operator ()(std::string file_name) {
  std::ifstream file;
  file.open(file_name);

  if (!file.is_open()) {
    std::cerr << "[ERROR] file " << file_name << " does not exists\n";
    return nullptr;
  }

  char c;
  unsigned n_line = 1;
  double v_x, v_y, v_z;
  long long i1, i2, i3;

  Mesh* aux_mesh = new Mesh ();
  bool end = false;
/*
  while (!file.eof()) {
    file >> c;
    switch (c) {
      case 'v':
        {
          file >> v_x;
          file >> v_y;
          file >> v_z;

          vertex_list.push_back(Point3{v_x, v_y, v_z});
        }
      break;
      case 'f':
        {
          file >> i1;
          file >> i2;
          file >> i3;

          // Fix posible negative indexes
          i1 = (i1 < 0 ? vertex_list.size() - i1 : i1 - 1);
          i2 = (i2 < 0 ? vertex_list.size() - i2 : i2 - 1);
          i3 = (i3 < 0 ? vertex_list.size() - i3 : i3 - 1);

          aux_mesh->local_coordinates_faces.push_back(Face3 {
                                vertex_list[i1],
                                vertex_list[i2],
                                vertex_list[i3],
                              });
        }
      break;

      default:
      break;
    }
  }
*/

  while (!end) {
    try {
      file >> c;
      if (c == 'v') {
        file >> v_x;
        file >> v_y;
        file >> v_z;
        vertex_list.push_back(Point3{v_x, v_y, v_z});
      } else {
        end = true;
      }
    } catch (...) {
      std::cerr << "[ERROR] Reading " << file_name << " bad formattin at line "
                << n_line << "\n";
      delete aux_mesh;
      return nullptr;
    }
    n_line++;
  }

  // Read faces
  while (!file.eof()) {
    try {
      if (c == 'f') {
        file >> i1;
        file >> i2;
        file >> i3;

        // Fix posible negative indexes
        i1 = (i1 < 0 ? vertex_list.size() - i1 : i1 - 1);
        i2 = (i2 < 0 ? vertex_list.size() - i2 : i2 - 1);
        i3 = (i3 < 0 ? vertex_list.size() - i3 : i3 - 1);

        if (i1 < 0 || i2 < 0 || i3 < 0)
          std::cout << "ARHC" << std::endl;

        aux_mesh->local_coordenates_faces.push_back(Face3 {
                              vertex_list[i1],
                              vertex_list[i2],
                              vertex_list[i3],
                            });
      } else {
        std::cerr << "[ERROR] Reading " << file_name << " expecting f at "
                  << n_line << "instead of " << c << "\n";
      }
    } catch (...) {
      std::cerr << "[ERROR] Readin " << file_name << " bad formattin at line "
                << n_line << "\n";
      delete aux_mesh;
      return nullptr;
    }
    file >> c;
    n_line++;
  }

  aux_mesh->generate_data();

  std::cout << "Succesfully loaded " << file_name << "\n"
            << " → Faces: " << aux_mesh->local_coordenates_faces.size() << "\n";

  return aux_mesh;
}
