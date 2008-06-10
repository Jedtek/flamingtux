/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is GPL, please see license.txt */
/* Compile instructions:
g++ $(pkg-config --libs --cflags gtkmm-2.4) $(pkg-config --libs --cflags libglademm-2.4) Main.cpp Application.cpp CommonFunctions.cpp gui/LoginWin.cpp gui/BuddyListWin.cpp gui/ConvoWin.cpp backend/FireClient.cpp backend/BackendEvents.cpp -o main -lxfirelib -lgthread-2.0 -lpthread

Needed Packages:
jam (to compile xfirelib)
xfirelib (see our modified version on the site)
libglademm development headers (e.g libglademm2.4-dev, itll drag in all the dependancies to make
it work)
Then you should be good!
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include "Main.h"
#include "Application.h"
#include <libglademm/xml.h>
#include <gtkmm.h>

using namespace std;
void quit_all() {
	Gtk::Main::quit();
}
int main(int argc, char *argv[]) {
	Glib::thread_init();
	Gtk::Main kit(argc, argv);
	
	Application fireapp = Application("gui/glade/flamingtux.glade");
	fireapp.createNewLoginWin();
	kit.run();
	return 0;
}

