
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
#include "backend/Log.h"
#include "backend/Config.h"

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
		Log *getLog() { return log_; }
		void setLog(Log *log) { log_ = log; }
		void deleteLog() { delete log_; }
		void setConfig(Config *config) { config_ = config; }
		Config *getConfig() { return config_; }
		void deleteConfig() { delete config_; }
		void deleteCreatedClasses() { delete log_; log_ = 0; delete config_; config_ = 0;}
		void showLoginWin();
		void deleteBuddyListWin();
		void createNewClient();
		void createNewLoginWin();
		void createNewConvoWin();
		void deleteConvoWin();
		void performLogOut();
		Gtk::Notebook_Helpers::PageIterator appendPageConvoWin(Gtk::TreeModel::iterator &iter);
		Glib::ustring getFlamingTuxDirectory() { return Glib::get_home_dir() + "/.flamingtux/"; }
		int checkConfigLogOption(Glib::ustring specific);
	private:
		void checkFlamingTuxDirectory();
		
		string path_;
		Gtk::Main *kit;
		Glib::RefPtr<Gnome::Glade::Xml> refXml_;
		FireClient *client_;
		Log *log_;
		Config *config_;
		
		LoginWin *loginWindow;
		BuddyListWin *buddyListWindow;
		ConvoWin *convoWindow;
};

#endif

