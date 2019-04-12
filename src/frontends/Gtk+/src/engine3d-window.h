/* engine3d-window.h
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

#pragma once

#include <gtkmm/builder.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/label.h>
#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>

#include "canvas.h"
#include "renderengine.h"
#include "../../../engine/rasteriser.h"
#include "../../../engine/camera.h"
#include "../../../engine/world.h"

class Engine3dWindow : public Gtk::Window
{
public:
	Engine3dWindow();	
private:
	Gtk::HeaderBar *headerbar;
	Gtk::Label *label;	
	Glib::RefPtr<Gtk::Builder> builder;
	Canvas canvas;
	Camera camera;
	World world;
	Rasteriser rasteriser;
	RenderEngine engine;
};
