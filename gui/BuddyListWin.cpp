#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

#include "BuddyListWin.h"

#include <libglademm/xml.h>
#include <gtkmm.h>

using namespace std;
using namespace xfirelib;
using namespace xfireclient;

// copied from http://www.parashift.com/c++-faq-lite/misc-technical-issues.html#faq-39.1
class BadConversion : public std::runtime_error {
	public:
		BadConversion(const std::string& s)
	: std::runtime_error(s)
		{ }
};
 
inline std::string stringify(int x)
{
	std::ostringstream o;
	if (!(o << x))
		throw BadConversion("stringify(double)");
	return o.str();
} 

BuddyListWin::BuddyListWin(Glib::RefPtr<Gnome::Glade::Xml> refXml, Application *app, FireClient *client)
	: buddylistwin_(0) {
	app_ptr_ = app;
	client_ = client;
	
	// get the buddy list window and its required widgetsy
	refXml->get_widget("BuddyListWin", buddylistwin_);
	if (!buddylistwin_)
		throw std::runtime_error("Couldn't find BuddyListWin");	
	refXml->get_widget("BuddyView", buddyview_);
	if (!buddyview_)
		throw std::runtime_error("Couldn't find BuddyView");
	refXml->get_widget("StatusEntry", statusentry_);
	if (!statusentry_)
		throw std::runtime_error("Couldn't find StatusEntry");
	refXml->get_widget("StatusComboBox", statuscombobox_);
	if (!statuscombobox_)
		throw std::runtime_error("Couldn't find StatusComboBox");
	
	// connect required signals
	statusentry_->signal_activate().connect(sigc::mem_fun(*this, &BuddyListWin::onStatusEntryChange));
	statuscombobox_->signal_changed().connect(sigc::mem_fun(*this, &BuddyListWin::onStatusEntryChange));
	
	// set default value
	statusentry_->set_text("Online");
	// remove the focus from the status entry widget
	buddylistwin_->set_focus((Gtk::Widget&) *buddyview_);
	
	createTreeModel();
	
	eventThread_ = new BEThread();
	eventThread_->signal_finished().connect(sigc::mem_fun(*this, &BuddyListWin::on_event_finish));
	// now we need to change the event thread for the client so it starts signalling with this one
	client_->setEventThread(eventThread_);
}

BuddyListWin::~BuddyListWin() {
	delete buddylistwin_;
}

void BuddyListWin::onStatusEntryChange() {
	cout << "Changing status..." << endl;
	SendStatusMessagePacket *packet = new SendStatusMessagePacket();
	//packet->awaymsg = joinString(cmds,1);//input.substr(5);
	client_->getClient()->send( packet );
	delete packet;
}

void BuddyListWin::createTreeModel() {
	buddystore_ = Gtk::TreeStore::create(m_Columns);
	buddyview_->set_model(buddystore_);
	buddyview_->set_has_tooltip();
	buddyview_->signal_query_tooltip().connect(sigc::mem_fun(*this, &BuddyListWin::onQueryTooltip));
	
	//buddyview_->set_reorderable();
	
	// Fill the TreeView's model
// 	online_row = *(buddystore_->append());
// 	online_row[m_Columns.m_col_name] = "Online";
// 	online_row[m_Columns.m_col_id] = 0;
// 	offline_row = *(buddystore_->append());
// 	offline_row[m_Columns.m_col_name] = "Offline";
// 	offline_row[m_Columns.m_col_id] = 1;
	// Add the TreeView's view columns
	buddyview_->append_column("Nickname", m_Columns.m_col_nickname);
	buddyview_->set_headers_visible(FALSE);
}

void BuddyListWin::addBuddyToTree(int id, Glib::ustring username, Glib::ustring nickname, Glib::ustring statusmsg, buddy_status status) {
	Gtk::TreeModel::Row childrow;
	if (status == ONLINE) {
		childrow = *(buddystore_->append(online_row.children()));
		if (nickname == "")
			childrow[m_Columns.m_col_nickname] = "<" + username + ">";
		else
			childrow[m_Columns.m_col_nickname] = nickname;
		childrow[m_Columns.m_col_username] = username;
		if (statusmsg == "")
			childrow[m_Columns.m_col_status] = "Online";
		else
			childrow[m_Columns.m_col_status] = statusmsg;
		childrow[m_Columns.m_col_id] = id;
	}
	else if (status == OFFLINE) {
		childrow = *(buddystore_->append(offline_row.children()));
		if (nickname == "")
			childrow[m_Columns.m_col_nickname] = "<" + username + ">";
		else
			childrow[m_Columns.m_col_nickname] = nickname;
		childrow[m_Columns.m_col_username] = username;
		childrow[m_Columns.m_col_status] = "Offline";
		childrow[m_Columns.m_col_id] = id;
	}
}
bool BuddyListWin::onQueryTooltip(int x, int y, bool keyboard_tooltip, const Glib::RefPtr<Gtk::Tooltip>& tooltip) {
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn* column;
	int cell_x;
	int cell_y; 
	buddyview_->get_path_at_pos(x, y, path, column, cell_x, cell_y);
	if(!path.gobj())
		return FALSE;
	// check if path is on "online" or "offline" node
	if (path.to_string() == "0" || path.to_string() == "1")
		return FALSE;
	Gtk::TreeModel::iterator iter = buddyview_->get_model()->get_iter(path);
	Glib::ustring text = (*iter)[m_Columns.m_col_username];
	text += "\nStatus : ";
	text += (*iter)[m_Columns.m_col_status];
	tooltip->set_markup("<b>" + text + "</b>");
	buddyview_->set_tooltip_row(tooltip, path);
	return TRUE;
}

void BuddyListWin::eventUpdateBuddyList() {
	buddystore_->clear();
	// Fill the TreeView's model
	online_row = *(buddystore_->append());
	//buddyview_->set_tooltip_text("lalala");
	online_row[m_Columns.m_col_nickname] = "Online";
	online_row[m_Columns.m_col_id] = 0;
	offline_row = *(buddystore_->append());
	offline_row[m_Columns.m_col_nickname] = "Offline";
	offline_row[m_Columns.m_col_id] = 1;
	
	vector<BuddyListEntry*> *entries = client_->getClient()->getBuddyList()->getEntries();
	for(uint i = 0 ; i < entries->size() ; i ++) {
		BuddyListEntry *entry = entries->at(i);
// 		printf("%1s %20s | %20s | %10ld | %20s | %7ld | %ld\n",
// 		       (entry->isOnline() ? "*" : ""),
// 			entry->username.c_str(),
// 					      entry->nick.c_str(),
// 							      entry->userid,
// 	     entry->statusmsg.c_str(),
// 				    entry->game,
// 	entry->game2);
		if (entry->isOnline())
			addBuddyToTree(entry->userid, entry->username, entry->nick, entry->statusmsg, ONLINE);
		else 
			addBuddyToTree(entry->userid, entry->username, entry->nick, entry->statusmsg, OFFLINE);
	}
}

void BuddyListWin::on_event_finish() {
	eventThread_->finish();
	cout << "--------------------------------------------" << endl;
	cout << "FINISHING EVENT BUDDYLIST" << endl;
	cout << "--------------------------------------------" << endl;
	switch (eventThread_->getType()) {
		case UPDATE_BUDDY_LIST: {
			eventUpdateBuddyList();
			break;
		}
		default:
			break;
	}
	eventThread_->setType(NOT_SET);
}
