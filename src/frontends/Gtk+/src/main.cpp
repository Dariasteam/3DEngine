/* main.cpp
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

#include <iostream>

#include "engine3d-window.h"

static void on_activate (Glib::RefPtr<Gtk::Application> app) {
	// Get the current window. If there is not one, we will create it.
	Gtk::Window *window = app->get_active_window();

	if (!window) {
		window = new Engine3dWindow();
		window->property_application() = app;
		window->property_default_width() = 800;
		window->property_default_height() = 600;
		app->add_window(*window);
	}
		
	// Ask the window manager/compositor to present the window to the user.
	window->present();
}

int main (int argc, char *argv[]) {
	int ret;

	// Create a new Gtk::Application. The application manages our main loop,
	// application windows, integration with the window manager/compositor, and
	// desktop features such as file opening and single-instance applications.
	Glib::RefPtr<Gtk::Application> app =
		Gtk::Application::create("org.gnome.Engine3D", Gio::APPLICATION_FLAGS_NONE);

	// We connect to the activate signal to create a window when the application
	// has been lauched. Additionally, this signal notifies us when the user
	// tries to launch a "second instance" of the application. When they try
	// to do that, we'll just present any existing window.
	//
	// Bind the app object to be passed to the callback "on_activate"
	app->signal_activate().connect(sigc::bind(&on_activate, app));

  //engine.main_loop();

	// Run the application. This function will block until the applicaiton
	// exits. Upon return, we have our exit code to return to the shell. (This
	// is the code you see when you do `echo $?` after running a command in a
	// terminal.
	ret = app->run(argc, argv);
			
	return ret;
}
