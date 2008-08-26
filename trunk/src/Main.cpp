/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is protected by GPLv3, please see LICENSE file for more information */
/* Compile instructions:
g++ $(pkg-config --libs --cflags gtkmm-2.4) $(pkg-config --libs --cflags libglademm-2.4) $(pkg-config --libs --cflags gtk+-2.0) $(pkg-config --libs --cflags gtkspell-2.0) Main.cpp Application.cpp CommonFunctions.cpp gui/LoginWin.cpp gui/BuddyListWin.cpp gui/ConvoWin.cpp gui/DialogWin.cpp gui/PreferencesWin.cpp backend/FireClient.cpp backend/BackendEvents.cpp backend/Log.cpp backend/Config.cpp backend/ConfigOptions.cpp backend/GtkHelperFunctions.cpp -o main -lxfirelib -lgthread-2.0 -lpthread

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
//#include "Main.h"
#include "Application.h"
#include <libglademm/xml.h>
#include <gtkmm.h>

using namespace std;

int main(int argc, char *argv[]) {
	Glib::thread_init();
	Gtk::Main kit(argc, argv);
	Application fireapp = Application(argc, argv);
	fireapp.run();
	kit.run();
	cout << "Exitting..." << endl;
	return 0;
}

