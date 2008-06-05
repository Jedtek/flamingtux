/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is GPL, licensing soon though ;) */
#ifndef ___APPLICATION_H___
#define ___APPLICATION_H___


#include <libglademm/xml.h>
#include <gtkmm.h>

#include "gui/BuddyListWin.h"
#include "gui/LoginWin.h"
#include "gui/ConvoWin.h"

using namespace std;

class Application {
	public:
		Application(string path);
		~Application();
		
		void getLoginWin();
		void createBuddyListWin();
		void hideLoginWin();
		FireClient *getClient() { return client_; }
		void createNewClient();
		void createNewLoginWin();
		void appendPageConvoWin(Gtk::TreeModel::iterator &iter);
	private:
		Glib::RefPtr<Gnome::Glade::Xml> refXml_;
		FireClient *client_;
		
		LoginWin *loginWindow;
		BuddyListWin *buddyListWindow;
		ConvoWin *convoWindow;
};

#endif

