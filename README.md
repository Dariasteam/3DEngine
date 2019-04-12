# Simple 3D engine

![](demo.gif)

This project's goal is to help me to understand the underlying maths used in geometric calculations. 
Because of that, it uses 3x3 matrices and Gauss equation system rather than uniform 4x4 matrices.

Project features two frontends which are responsible of finally drawing a std::vector<Triangle>

- Gtk+:
		cd src/frontends/Gtk+/		
		meson build
		cd build
		ninja
		./src/engine3D
- Qt 5: 
		cd src/frontends/Qt/
		qmake
		make -j 
		./3D		
