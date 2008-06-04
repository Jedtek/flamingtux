/* Compile instructions:
g++ $(pkg-config --libs --cflags gtkmm-2.4) $(pkg-config --libs --cflags libglademm-2.4) Main.cpp Application.cpp gui/LoginWin.cpp gui/BuddyListWin.cpp backend/FireClient.cpp backend/BackendEvents.cpp -o main -lxfirelib -lgthread-2.0 -lpthread
*/

#include <iostream>
#include <stdexcept>
#include <string>

#include "Application.h"
#include <libglademm/xml.h>
#include <gtkmm.h>

using namespace std;

int main(int argc, char *argv[]) {
	Glib::thread_init();
	Gtk::Main kit(argc, argv);
	
	Application fireapp = Application("gui/glade/fire4lin.glade");
	fireapp.createNewLoginWin();
	kit.run();
	return 0;
}
