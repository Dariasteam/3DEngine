#include "textureprojector.h"

TextureProjector::TextureProjector() :
  screen(600, 300)
{}

void TextureProjector::project(const Texture& t, Point2i p, Point2i v, Point2i u) {
  std::cout << "Final screen" << std::endl;

  // Base en la textura original (destino)
  Basis2 b1 ({
               {0,   300},
               {600, 300}
             });

  // Base en la pantalla (origen)
  Basis2 b2 ({
               {double(v.X - p.X), double(v.Y - p.Y)},
               {double(u.X - p.X), double(u.Y - p.Y)},
             });

  // Generate matrix to change between basis
  Matrix2 basis_changer;
  MatrixOps::generate_basis_change_matrix(b2, b1, basis_changer);

  std::cout << "Matrix\n" << basis_changer[0][0] << " "
                          << basis_changer[0][1] << "\n"
                          << basis_changer[1][0] << " "
                          << basis_changer[1][1] << "\n";

  // Populate screen
  for (int i = 0; i < screen.get_height(); i++) {
    for (int j = 0; j < screen.get_width(); j++) {
      Matrix m ({
                  {double(j - p.X), double(i - p.Y)}
                });

      auto m2 = m * basis_changer;

      int x = std::round(m2[0][0]);
      int y = std::round(m2[0][1]);
/*
      std::cout << i << " " << j << " -> "
                << x << " " << y << std::endl;
*/
      if (x >= 0 && x <= 600 && y >= 0 && y <= 300) {
        screen.set(j, i, 0, t.get(x, y, 0));
        screen.set(j, i, 1, t.get(x, y, 1));
        screen.set(j, i, 2, t.get(x, y, 2));
      } else {
        std::cout << "Error: " << x << " " << y << std::endl;
      }
    }
  }

  screen.write("screen.ppm");
}

void TextureProjector::write() const {
  //screen.write("screen.ppm");
}
