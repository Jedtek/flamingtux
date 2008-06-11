/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 *  This code is protected by GPLv3, please see LICENSE file for more information */
#ifndef ___BUDDYLISTWIN_H___
#define ___BUDDYLISTWIN_H___

#include <libglademm/xml.h>
#include <gtkmm.h>

#include "../ModelColumns.h"
#include "../backend/FireClient.h"
#include "../backend/BackendEvents.h"
class Application;

enum buddy_status { ONLINE, OFFLINE };

class BuddyListWin : public Gtk::Window {
	public:
		explicit BuddyListWin(Glib::RefPtr<Gnome::Glade::Xml> refXml, Application *app, FireClient *client);
		~BuddyListWin();

		Gtk::Window &get_window() const { return *buddylistwin_; }
		void addBuddyToTree(int id, Glib::ustring username, Glib::ustring nickname, Glib::ustring statusmsg, buddy_status status);
		
		void eventUpdateBuddyList();
		void set_client(FireClient *client) { client_ = client; }
		/* im just gunna shove my invite functions here :D - Jeed */
		void onInviteRecieved();
		/* Like youll stop me :D */
		void onStatusEntryChange();
		bool onQueryTooltip(int x, int y, bool keyboard_tooltip, const Glib::RefPtr<Gtk::Tooltip>& tooltip);
		Gtk::TreeIter &get_iter_at_username(Glib::ustring username);
	private:
		void onInviteBtnClicked();
		void onInviteMenuItemActivate();
		void onQuitActivate();
		virtual bool onDeleteEvent(GdkEventAny *e);
		Glib::RefPtr<Gnome::Glade::Xml> refXml_;
		Gtk::Window *buddylistwin_;
		Gtk::Entry *statusentry_;
		Gtk::ComboBoxEntry *statuscombobox_;
		Gtk::ImageMenuItem *quit_widget_;
		Gtk::MenuItem *invite_buddy_widget_;
		BEThread *eventThread_;
		
		ModelColumns m_Columns;
		
		Gtk::TreeModel::Row online_row;
		Gtk::TreeModel::Row offline_row;
		
		Gtk::TreeView *buddyview_;
		Glib::RefPtr<Gtk::TreeStore> buddystore_;	
		
		Gtk::Window *invitesendwin_;
		Gtk::Entry *invitesendwho_;
		Gtk::Button *invitesendbtn_;
		Gtk::Entry *invitesendmessage_;
		
		Application *app_ptr_;
		FireClient *client_;
		void on_event_finish();
		void createTreeModel();
		void onMessageReceived();
		void on_treeview_clicked(GdkEventButton *event);
		void on_treeview_keyed(GdkEventKey *event);
		void onStatusEntryKeyRelease(GdkEventKey* event);

};

#endif
