/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 *  This code is protected by GPLv3, please see LICENSE file for more information */
#ifndef ___LOGINWIN_H___
#define ___LOGINWIN_H___

#include <libglademm/xml.h>
#include <gtkmm.h>

#include "FireClient.h"
#include "BackendEvents.h"

using namespace std;

class Application;

class LoginWin : public Gtk::Window {
	public:
		explicit LoginWin(Glib::RefPtr<Gnome::Glade::Xml> refXml, Application *app, FireClient *client);
		~LoginWin();

		
		Gtk::Window &get_window() const { return *loginwin_; }
		FireClient *get_client() { return client_; }
		void setClient(FireClient *client) { client_ = client; }
		void login_pressed();
		
		void eventLoginFailed();
		void eventLoginSuccess();
		
	private:
		bool onProgressBarTimeout();
		virtual bool onDeleteEvent(GdkEventAny *e);
		int createUserDirectoryStructure(Glib::ustring username, Glib::ustring ft_directory);
		Gtk::Window *loginwin_;
		Gtk::Entry *user_entry_;
		Gtk::Entry *pass_entry_;
		Gtk::Button *login_btn_;
		Gtk::ProgressBar *progress_bar_;
		BEThread *eventThread_;
		Glib::RefPtr<Gnome::Glade::Xml> refXml_;
		Application *app_ptr_;
		FireClient *client_;
		void on_event_finish();
		int timeoutConnectionId;
};

#endif


