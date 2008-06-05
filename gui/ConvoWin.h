/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is GPL, licensing soon though ;) */
#ifndef __CONVOWIN__H
#define __CONVOWIN__H

#include <libglademm/xml.h>
#include <gtkmm.h>

#include "../backend/FireClient.h"
#include "../backend/BackendEvents.h"

using namespace std;

class Application;

class ConvoWin : public Gtk::Window {
	public:
		explicit ConvoWin(Glib::RefPtr<Gnome::Glade::Xml> refXml, Application *app, FireClient *client);
		~ConvoWin();

		Gtk::Window &get_window() const { return *convowin_; }
		int appendPage(Gtk::TreeModel::iterator &iter);
		
	private:
		Gtk::Window *convowin_;
		Glib::RefPtr<Gnome::Glade::Xml> refXml_;
		Gtk::Notebook *convonotebook_;
		Gtk::VBox *convowinvbox_;
		Application *app_ptr_;
		FireClient *client_;
};

#endif

