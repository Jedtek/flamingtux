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
		Gtk::Notebook_Helpers::PageIterator appendPage(Gtk::TreeModel::iterator &iter);
		void gotMessage(Gtk::TreeIter &iter, Glib::ustring message);
		void updatePage(Gtk::TreeModel::Row childrow);
	private:
		void onCloseMenuItem();
		void updateStatusTextView(Gtk::TextView *text_view, Glib::ustring nickname, int status);
		void closeTab(Gtk::VBox *notebook_vbox);
		void onCloseTabClicked(Gtk::VBox *notebook_vbox);
		virtual bool onDeleteEvent(GdkEventAny *e);
		void onWindowStateEvent(GdkEventWindowState* event);
		void onPageRemoved(Gtk::Widget* page, guint page_num);
		void onPageSwitched(GtkNotebookPage* page, guint page_num);
		void onSendButtonPressed(Gtk::VBox *vbox, Gtk::TreeModel::iterator &iter);
		void updateTextView(Gtk::TextView *text_view, Glib::ustring nickname, Glib::ustring text);
		void onVScrollValueChange(Gtk::ScrolledWindow *scrolled_win);
		Gtk::Window *convowin_;
		Glib::RefPtr<Gnome::Glade::Xml> refXml_;
		Gtk::Notebook *convonotebook_;
		Gtk::VBox *convowinvbox_;
		Application *app_ptr_;
		FireClient *client_;
		Gtk::ImageMenuItem *close_widget_;
		int windowMinimized;
		int windowAbove;
};

#endif
