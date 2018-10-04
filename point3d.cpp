#include "point3d.h"


Mesh *Mesh::express_in_different_basis(const Basis3 &new_basis) const {

  Mesh* aux_mesh = new Mesh (*this);

  aux_mesh->basis = new_basis;

  // Calcular matriz de cambio de base
  Matrix3 basis_changer = MatrixOps::
      generate_basis_change_matrix(basis, new_basis);


  for (unsigned i = 0; i < nested_meshes.size(); i++) {
      Mesh* aux_nested_mesh = nested_meshes[i]->express_in_different_basis(new_basis);

      unsigned base_size = aux_mesh->faces.size();
      unsigned nested_size = aux_nested_mesh->faces.size();

      aux_mesh->faces.resize(base_size + nested_size);

      for (unsigned j = 0; j < nested_size; j++) {
        aux_mesh->faces[base_size + j] = aux_nested_mesh->faces[j];
      }
      delete aux_nested_mesh;
    }

  for (unsigned i = 0; i < aux_mesh->faces.size(); i++) {
      Face3 aux;
      for (unsigned j = 0; j < 3; j++) {
          aux_mesh->faces[i][j] = MatrixOps::change_basis(basis_changer, aux_mesh->faces[i][j]);
          aux_mesh->faces[i][j] += position;
        }
    }

  return aux_mesh;
}
