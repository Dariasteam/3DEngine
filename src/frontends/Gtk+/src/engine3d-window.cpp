/* engine3d-window.cpp
 *
 * Copyright 2018 dariasteam
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "engine3d-window.h"

Engine3dWindow::Engine3dWindow()
	: Glib::ObjectBase("Engine3dWindow")
	, Gtk::Window()
	, headerbar(nullptr)
	, label(nullptr)		
	, world (&camera)
	, rasteriser (&canvas, &camera, &world)
	, engine (&rasteriser, &camera, &world)
{
	builder = Gtk::Builder::create_from_resource("/org/gnome/Engine3D/engine3d-window.ui");
	builder->get_widget("headerbar", headerbar);		
	set_titlebar(*headerbar);
	headerbar->show();
	add(canvas);
	canvas.show();		
}