/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 *  This code is protected by GPLv3, please see LICENSE file for more information */
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
		Application(int argc, char *argv[], string path);
		~Application();
		
		void run();
		void quit();
		BuddyListWin *getBuddyListWin() { return buddyListWindow; }
		ConvoWin *getConvoWin() { return convoWindow; }
		void createBuddyListWin();
		void hideLoginWin();
		FireClient *getClient() { return client_; }
		void createNewClient();
		void createNewLoginWin();
		void createNewConvoWin();
		Gtk::Notebook_Helpers::PageIterator appendPageConvoWin(Gtk::TreeModel::iterator &iter);
	private:
		Gtk::Main *kit;
		Glib::RefPtr<Gnome::Glade::Xml> refXml_;
		FireClient *client_;
		
		LoginWin *loginWindow;
		BuddyListWin *buddyListWindow;
		ConvoWin *convoWindow;
};

#endif

