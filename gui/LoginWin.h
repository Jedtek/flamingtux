/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is GPL, licensing soon though ;) */
#ifndef ___LOGINWIN_H___
#define ___LOGINWIN_H___

#include <libglademm/xml.h>
#include <gtkmm.h>

#include "../backend/FireClient.h"
#include "../backend/BackendEvents.h"

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
		virtual bool onDeleteEvent(GdkEventAny *e);
		
		Gtk::Window *loginwin_;
		Gtk::Entry *user_entry_;
		Gtk::Entry *pass_entry_;
		Gtk::Button *login_btn_;
		BEThread *eventThread_;
		Glib::RefPtr<Gnome::Glade::Xml> refXml_;
		Application *app_ptr_;
		FireClient *client_;
		void on_event_finish();
};

#endif


