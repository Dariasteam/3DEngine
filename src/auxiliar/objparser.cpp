#include "objparser.h"


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

  // Vertices
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

  // Copy points of the vertices
  aux_mesh->vertices.resize(vertex_list.size());
  for (unsigned i = 0; i < vertex_list.size(); i++) {
    aux_mesh->vertices[i].point_local = vertex_list[i];
  }

  std::vector<Vector3> vertex_normals (aux_mesh->vertices.size(), {0, 0, 0});

  // Read faces
  while (!file.eof()) {
    try {
      if (c == 'f') {
        file >> i1;
        file >> i2;
        file >> i3;

        i1--;
        i2--;
        i3--;

        Face tmp_face = Face {
                              &aux_mesh->vertices[i1],
                              &aux_mesh->vertices[i2],
                              &aux_mesh->vertices[i3],
                            };

        tmp_face.generate_local_normal();
        Vector3 normal = tmp_face.normal_local.toVector3();

        vertex_normals[i1] += normal;
        vertex_normals[i1] += normal;
        vertex_normals[i1] += normal;

        aux_mesh->faces.push_back(tmp_face);
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

  std::cout << "Generating vertex normals" << std::endl;
  // Per vertex normals
  for (int i = 0; i < vertex_normals.size(); i++) {
    vertex_normals[i].normalize();
    aux_mesh->vertices[i].normal_local = Normal3(vertex_normals[i]);
  }

  std::cout << "Succesfully loaded " << file_name << "\n"
            << " → Faces: " << aux_mesh->faces.size() << "\n";

  return aux_mesh;
}
