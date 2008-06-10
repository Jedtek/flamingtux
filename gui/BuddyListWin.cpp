/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 *  This code is protected by GPLv3, please see LICENSE file for more information */
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

#include "BuddyListWin.h"
#include "../Application.h"
#include "../CommonFunctions.h"
#include <libglademm/xml.h>
#include <gtkmm.h>

using namespace std;
using namespace xfirelib;
using namespace xfireclient;

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
	
	refXml->get_widget("BuddyListWinQuit", quit_widget_);
	if (!quit_widget_)
		throw std::runtime_error("Couldn't find BuddyListWinQuit");
	
	refXml->get_widget("InviteBuddyBuddyListWin", invite_buddy_widget_);
	if (!invite_buddy_widget_)
		throw std::runtime_error("Couldn't find InviteBuddyBuddyListWin");
	
	refXml->get_widget("InviteSendWin", invitesendwin_);
	if (!invitesendwin_)
		throw std::runtime_error("Couldn't find InviteSendWin");
	refXml->get_widget("InviteSendWho", invitesendwho_);
	if (!invitesendwho_)
		throw std::runtime_error("Couldn't find InviteSendWho");
	refXml->get_widget("InviteSendMessage", invitesendmessage_);
	if (!invitesendmessage_)
		throw std::runtime_error("Couldn't find InviteSendMessage");
	refXml->get_widget("InviteSendBtn", invitesendbtn_);
	if (!invitesendbtn_)
		throw std::runtime_error("Couldn't find InviteSendBtn");
	invitesendbtn_->signal_clicked().connect(sigc::mem_fun(*this, &BuddyListWin::onInviteBtnClicked));
	
	// connect required signals
	statusentry_->signal_key_release_event().connect_notify(sigc::mem_fun(*this, &BuddyListWin::onStatusEntryKeyRelease));
	statuscombobox_->signal_changed().connect(sigc::mem_fun(*this, &BuddyListWin::onStatusEntryChange));
	buddylistwin_->signal_delete_event().connect(sigc::mem_fun(*this, &BuddyListWin::onDeleteEvent));
	quit_widget_->signal_activate().connect(sigc::mem_fun(*this, &BuddyListWin::onQuitActivate));
	invite_buddy_widget_->signal_activate().connect(sigc::mem_fun(*this, &BuddyListWin::onInviteMenuItemActivate));
	// set default value
	statusentry_->set_text("Online");
	// remove the focus from the status entry widget
	buddylistwin_->set_focus((Gtk::Widget&) *buddyview_);
	
	createTreeModel();
	
	eventThread_ = new BEThread();
	eventThread_->signal_finished().connect(sigc::mem_fun(*this, &BuddyListWin::on_event_finish));
	// now we need to change the event thread for the client so it starts signalling with this one
	client_->setEventThread(eventThread_);
	/* set this for use elsewhere in this file */
	refXml_ = refXml;
}

BuddyListWin::~BuddyListWin() {
	delete buddylistwin_;
}

void BuddyListWin::onStatusEntryKeyRelease(GdkEventKey* event) {
	if (event->keyval == GDK_Return) {
		cout << "Changing status..." << endl;
		SendStatusMessagePacket *packet = new SendStatusMessagePacket();
		packet->awaymsg = statusentry_->get_text();
		client_->getClient()->send( packet );
		delete packet;
	}
}

void BuddyListWin::onQuitActivate() {
	onDeleteEvent(NULL);
}

void BuddyListWin::onStatusEntryChange() {
	if (!(statuscombobox_->get_active_row_number() == -1)) {
		cout << "Changing status..." << endl;
		SendStatusMessagePacket *packet = new SendStatusMessagePacket();
		packet->awaymsg = statusentry_->get_text();
		client_->getClient()->send( packet );
		delete packet;
	}
}
/* INVITE FUNCTIONS */
void BuddyListWin::onInviteRecieved() {
	cout << "Invite recieved..spawning dialouge" << endl;
	vector<InviteRequestPacket *> inviteVector = client_->getInviteVector();
	for (int i = 0; i < inviteVector.size(); i++) {
		InviteRequestPacket *invite = inviteVector.at(i);
		Gtk::MessageDialog invite_accept(*this, "You have a new invite request from:", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
		invite_accept.set_secondary_text("User: " + invite->name + " Nickname:" + invite->nick + " Invite Message:" + invite->msg);
		int i_result = invite_accept.run();
		switch(i_result) {
			case(Gtk::RESPONSE_YES): {
				/* better let that pesky backend know */
				InviteBuddyPacket *invite_accepted = new InviteBuddyPacket();
				invite_accepted->InviteBuddyPacket::addInviteName(invite->name, "Accepted.");
				client_->getClient()->send(invite_accepted);
			}
			case(Gtk::RESPONSE_NO): {
				/* Seems you do nothing here.. */
				break;
			}
		delete invite;
		}
	}
	client_->clearInviteVector();
}
void BuddyListWin::onInviteMenuItemActivate() {
	/* Lets spawn a funky window to get the username they want to add! 
	 * Why dont gtk+/gtkmm implement a damn text entry dialouge, it'd be so much easier */
	invitesendwin_->show();

}

void BuddyListWin::onInviteBtnClicked() {
	Glib::ustring username = invitesendwho_->get_text();
	Glib::ustring message = invitesendmessage_->get_text();
	cout << "we want to invite this bad boy: " << username << endl;
	InviteBuddyPacket invite_sender;
	invite_sender.addInviteName(username, message);
	int ireturn = client_->getClient()->send(&invite_sender);
	Gtk::MessageDialog dialog(*this, "Invite Buddy", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK);
	if (ireturn)
		dialog.set_secondary_text("Successfully invited " + username);
	else
		dialog.set_secondary_text("Error inviting " + username);	
	dialog.run();
	invitesendwin_->hide();
	invitesendwho_->set_text("");
	invitesendmessage_->set_text("");
}

bool BuddyListWin::onDeleteEvent(GdkEventAny *e) {
	Gtk::MessageDialog close_dialog(*this, "Quit FalmingTux", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
	close_dialog.set_secondary_text("Are you sure you want to quit?");
	int d_result = close_dialog.run();
	switch(d_result) {
		case(Gtk::RESPONSE_YES): {
			int i;
			client_->getClient()->disconnect();
			app_ptr_->quit();
			return false;
			break;
		}
		case(Gtk::RESPONSE_NO): {
			/* Do nothing and return true */
			return true;
			break;
		}
	}
}

void BuddyListWin::createTreeModel() {
	buddystore_ = Gtk::TreeStore::create(m_Columns);
	buddyview_->set_model(buddystore_);
	buddyview_->set_has_tooltip();
	buddyview_->signal_query_tooltip().connect(sigc::mem_fun(*this, &BuddyListWin::onQueryTooltip));
	
	Gtk::CellRendererText& nickname_renderer = *manage (new Gtk::CellRendererText());
	Gtk::TreeViewColumn& nickname_column = *manage (new Gtk::TreeViewColumn ("Nickname", nickname_renderer));

	nickname_column.add_attribute(nickname_renderer.property_markup(), m_Columns.m_col_nickname_rendered);
	//name_column.add_attribute (name_renderer.property_weight (),_category_columns._name_weight);

	buddyview_->append_column(nickname_column); 
	
	buddyview_->add_events(Gdk::BUTTON_PRESS_MASK);
	buddyview_->signal_button_press_event().connect_notify(
					      sigc::mem_fun(*this, &BuddyListWin::on_treeview_clicked));
	buddyview_->signal_key_press_event().connect_notify(
					      sigc::mem_fun(*this, &BuddyListWin::on_treeview_keyed));
	//buddyview_->set_reorderable();
	
	// Fill the TreeView's model
// 	online_row = *(buddystore_->append());
// 	online_row[m_Columns.m_col_name] = "Online";
// 	online_row[m_Columns.m_col_id] = 0;
// 	offline_row = *(buddystore_->append());
// 	offline_row[m_Columns.m_col_name] = "Offline";
// 	offline_row[m_Columns.m_col_id] = 1;
	// Add the TreeView's view columns
	//buddyview_->append_column("Nickname", m_Columns.m_col_nickname);
	buddyview_->set_headers_visible(FALSE);
}

void BuddyListWin::on_treeview_clicked(GdkEventButton *event) {
	ModelColumns m_Columns;
	if (event->type == GDK_2BUTTON_PRESS) {
		Glib::RefPtr<Gtk::TreeView::Selection> refSelection = buddyview_->get_selection();
		if(refSelection) {
			Gtk::TreeModel::iterator iter = refSelection->get_selected();
			if(iter && (*iter)[m_Columns.m_col_id] != 0 && (*iter)[m_Columns.m_col_id] != 1) {
				app_ptr_->appendPageConvoWin(iter);	
			}
		}
	}
}

void BuddyListWin::on_treeview_keyed(GdkEventKey *event) {
	ModelColumns m_Columns;
	if (event->keyval == GDK_Return) {
		Glib::RefPtr<Gtk::TreeView::Selection> refSelection = buddyview_->get_selection();
		if(refSelection) {
			Gtk::TreeModel::iterator iter = refSelection->get_selected();
			if(iter && (*iter)[m_Columns.m_col_id] != 0 && (*iter)[m_Columns.m_col_id] != 1) {
				app_ptr_->appendPageConvoWin(iter);	
			}
		}
	}
}

void BuddyListWin::addBuddyToTree(int id, Glib::ustring username, Glib::ustring nickname, Glib::ustring statusmsg, buddy_status status) {
	Gtk::TreeModel::Row childrow;
	if (status == ONLINE) {
		childrow = *(buddystore_->append(online_row.children()));
		if (nickname == "") {
			childrow[m_Columns.m_col_nickname_rendered] = "<span color='Dark Green'>[" + username + "]</span>";
			childrow[m_Columns.m_col_nickname] = "[" + username + "]";
		}
		else {
			childrow[m_Columns.m_col_nickname_rendered] = "<span color='Dark Green'>" + nickname + "</span>";
			childrow[m_Columns.m_col_nickname] = nickname;
		}
		childrow[m_Columns.m_col_username] = username;
		if (statusmsg == "")
			childrow[m_Columns.m_col_status] = "Online";
		else
			childrow[m_Columns.m_col_status] = statusmsg;
		childrow[m_Columns.m_col_id] = id;
	}
	else if (status == OFFLINE) {
		childrow = *(buddystore_->append(offline_row.children()));
		if (nickname == "") {
			childrow[m_Columns.m_col_nickname_rendered] = "<span color='Dark Blue'>[" + username + "]</span>";
			childrow[m_Columns.m_col_nickname] = "[" + username + "]";
		}
		else {
			childrow[m_Columns.m_col_nickname_rendered] = "<span color='Dark Blue'>" + nickname + "</span>";
			childrow[m_Columns.m_col_nickname] = nickname;
		}
		childrow[m_Columns.m_col_username] = username;
		childrow[m_Columns.m_col_status] = "Offline";
		childrow[m_Columns.m_col_id] = id;
	}
	if (app_ptr_->getConvoWin())
		app_ptr_->getConvoWin()->updatePage(childrow);
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
	Glib::ustring text;
	text += "<b>" + (*iter)[m_Columns.m_col_nickname] + "</b>\n";
	text += (*iter)[m_Columns.m_col_username];
	text += "\nStatus : ";
	text += (*iter)[m_Columns.m_col_status];
	tooltip->set_markup(text);
	buddyview_->set_tooltip_row(tooltip, path);
	return TRUE;
}

void BuddyListWin::eventUpdateBuddyList() {
	vector<BuddyListEntry*> *entries = client_->getClient()->getBuddyList()->getEntries();
	int noOfOnlineEntries = 0;
	buddystore_->clear();
	// Fill the TreeView's model
	online_row = *(buddystore_->append());
	//buddyview_->set_tooltip_text("lalala");
	online_row[m_Columns.m_col_id] = 0;
	offline_row = *(buddystore_->append());
	offline_row[m_Columns.m_col_id] = 1;
	
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
		if (entry->isOnline()) {
			addBuddyToTree(entry->userid, entry->username, entry->nick, entry->statusmsg, ONLINE);
			noOfOnlineEntries += 1;
		}
		else 
			addBuddyToTree(entry->userid, entry->username, entry->nick, entry->statusmsg, OFFLINE);
	}
	online_row[m_Columns.m_col_nickname_rendered] = "<b>Online (" + stringify(noOfOnlineEntries) + ")</b>";
	offline_row[m_Columns.m_col_nickname_rendered] = "<b>Offline (" + stringify(entries->size() - noOfOnlineEntries) + ")</b>";
	buddyview_->expand_all();
}

void BuddyListWin::on_event_finish() {
	cout << "--------------------------------------------" << endl;
	cout << "FINISHING EVENT BUDDYLIST" << endl;
	cout << "--------------------------------------------" << endl;
	switch (eventThread_->getType()) {
		case UPDATE_BUDDY_LIST: {
			eventUpdateBuddyList();
			break;
		}
		case INVITE_REQUEST: {
			onInviteRecieved();
			break;
		}
		case GOT_MESSAGE: {
			onMessageReceived();
			break;
		}
		default:
			break;
	}
	eventThread_->setType(NOT_SET);
	eventThread_->finish();
}

Gtk::TreeIter &BuddyListWin::get_iter_at_username(Glib::ustring username) {
	ModelColumns m_Columns;
	
	Gtk::TreeModel::Children children = online_row.children();
	Gtk::TreeIter *i = new Gtk::TreeIter();
	for (*i = children.begin(); *i != children.end(); (*i)++) {
		if ((**i)[m_Columns.m_col_username] == username)
			return *i;
	}
}

void BuddyListWin::onMessageReceived() {
	ModelColumns m_Columns;
	vector<MessagePacket *> vector = client_->getMessageVector();
	for (int i = 0; i < vector.size(); i++) {
		MessagePacket *message = vector.at(i);
		BuddyListEntry *entry = client_->getClient()->getBuddyList()->getBuddyBySid( message->getSid() );
		Gtk::TreeIter &iter = get_iter_at_username(entry->username);
		if (!app_ptr_->getConvoWin())
			app_ptr_->createNewConvoWin();
		app_ptr_->getConvoWin()->gotMessage(iter, message->getMessage());
		delete message;
	}
	client_->clearMessageVector();
}
