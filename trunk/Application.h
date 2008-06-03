#ifndef ___APPLICATION_H___
#define ___APPLICATION_H___


#include <libglademm/xml.h>
#include <gtkmm.h>

#include "gui/BuddyListWin.h"
#include "gui/LoginWin.h"

using namespace std;

class Application {
	public:
		Application(string path);
		~Application();
		
		void getLoginWin();
		void getBuddyListWin();
		void hideLoginWin();
	private:
		Gtk::Window *loginwin_;
		Gtk::Window *buddylistwin_;
		Glib::RefPtr<Gnome::Glade::Xml> refXml_;
		FireClient *client_;
		
		LoginWin *loginWindow;
		BuddyListWin *buddyListWindow;
};

#endif

