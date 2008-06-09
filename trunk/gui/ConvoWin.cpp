/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is GPL, licensing soon though ;) */
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

#include "ConvoWin.h"

#include <libglademm/xml.h>
#include <gtkmm.h>
#include "../Application.h"
#include "../ModelColumns.h"
#include "../CommonFunctions.h"

using namespace std;
using namespace xfirelib;
using namespace xfireclient;

ConvoWin::ConvoWin(Glib::RefPtr<Gnome::Glade::Xml> refXml, Application *app, FireClient *client)
	: convowin_(0) {
	app_ptr_ = app;
	client_ = client;
	refXml_ = refXml;
	
	refXml->get_widget("ConvoWin", convowin_);
	if (!convowin_)
		throw std::runtime_error("Couldn't find ConvoWin");	
	refXml->get_widget("ConvoWinVBox", convowinvbox_);
	if (!convowinvbox_)
		throw std::runtime_error("Couldn't find ConvoWinVBox");
	
	convonotebook_ = new Gtk::Notebook();
	convowinvbox_->pack_end(*convonotebook_, true, true, 0);

}

ConvoWin::~ConvoWin() {
	
}

int ConvoWin::appendPage(Gtk::TreeModel::iterator &iter) {
	ModelColumns m_Columns;
	
	cout << (*iter)[m_Columns.m_col_nickname] << endl;
	/* check if a convo is already opened for the user */
	if (convonotebook_->get_n_pages()) {
		Gtk::Notebook_Helpers::PageIterator i;
		Gtk::VBox *vbox;
		Gtk::Box_Helpers::BoxList::iterator bi;
		Gtk::Label *label_i;
		for(i = convonotebook_->pages().begin(); i != convonotebook_->pages().end(); i++) {
			vbox = dynamic_cast<Gtk::VBox*>(i->get_child());
			bi = vbox->children().begin();
			label_i = dynamic_cast<Gtk::Label*>(bi->get_widget());
			if (stringify((*iter)[m_Columns.m_col_id]) == label_i->get_text()) {
				convonotebook_->set_current_page(convonotebook_->page_num(*vbox));
				return 0;
			}
		}
	}
	
	/* create the new child widget */
	Gtk::VBox *notebook_vbox = new Gtk::VBox();
	Gtk::HBox *entry_hbox = new Gtk::HBox();
	Gtk::Button *entry_btn = new Gtk::Button("Send");
	Gtk::Label *label = new Gtk::Label("To: " + (*iter)[m_Columns.m_col_nickname] + " <" + (*iter)[m_Columns.m_col_status] + '>');
	Gtk::Label *label_id = new Gtk::Label();
	Gtk::Label *label_nick = new Gtk::Label((*iter)[m_Columns.m_col_nickname]);
	label_id->set_label(stringify((*iter)[m_Columns.m_col_id]));
	Gtk::ScrolledWindow *scrolled_win = new Gtk::ScrolledWindow();
	Gtk::ScrolledWindow *scrolled_win2 = new Gtk::ScrolledWindow();
	Gtk::TextView *text_view = new Gtk::TextView();
	text_view->set_editable(false);
	Gtk::TextView *text_view2 = new Gtk::TextView();
	//text_view2->set_flags(~Gtk::CAN_FOCUS);
	scrolled_win->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scrolled_win->add(*text_view);
	scrolled_win2->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scrolled_win2->add(*text_view2);
	notebook_vbox->pack_start(*label_id, false, false, 0);
	notebook_vbox->pack_start(*label_nick, false, false, 0);
	notebook_vbox->pack_start(*label, false, false, 0);
	notebook_vbox->pack_start(*scrolled_win, true, true, 0);
	notebook_vbox->pack_start(*entry_hbox, false, false, 5);
	entry_hbox->pack_start(*scrolled_win2, true, true, 0);
	entry_hbox->pack_start(*entry_btn, false, false, 0);
	
	scrolled_win->get_vscrollbar()->get_adjustment()->signal_changed().connect(sigc::bind(mem_fun(*this,&ConvoWin::onVScrollValueChange), scrolled_win));
	
	/* add it to the notebook */
	convonotebook_->append_page(*notebook_vbox, (*iter)[m_Columns.m_col_username]);
	convonotebook_->show();
	notebook_vbox->show();
	label->show();
	scrolled_win->show();
	text_view->show();
	scrolled_win2->show();
	text_view2->show();
	entry_hbox->show();
	entry_btn->show();
	
	if ((*iter)[m_Columns.m_col_status] == "Offline") {
		text_view2->get_buffer()->set_text("The user is currently offline");
		text_view2->set_sensitive(false);
	}
	
	/* add the button signal */
	entry_btn->add_accelerator("activate", convowin_->get_accel_group(), GDK_Return, 
				   (Gdk::ModifierType) 0, Gtk::ACCEL_VISIBLE);
	entry_btn->signal_pressed().connect(sigc::bind(sigc::mem_fun(*this, &ConvoWin::onSendButtonPressed), notebook_vbox, iter));
	entry_btn->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &ConvoWin::onSendButtonPressed), notebook_vbox, iter));
	
	convonotebook_->set_current_page(convonotebook_->page_num(*notebook_vbox));
				 
	return 0;
}

void ConvoWin::onSendButtonPressed(Gtk::VBox *vbox, Gtk::TreeModel::iterator &iter) {
	ModelColumns m_Columns;
	
	Gtk::Box_Helpers::BoxList::iterator wi = vbox->children().begin();
	Gtk::Label *label_id = dynamic_cast<Gtk::Label*>(wi->get_widget());
	Gtk::Label *label_nick = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::Label *label = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::ScrolledWindow *scrolled_win = dynamic_cast<Gtk::ScrolledWindow*>((++wi)->get_widget());
	Gtk::TextView *text_view = dynamic_cast<Gtk::TextView*>(*(scrolled_win->get_children().begin()));
	Gtk::HBox *entry_hbox = dynamic_cast<Gtk::HBox*>((++wi)->get_widget());
	Gtk::ScrolledWindow *scrolled_win2 = dynamic_cast<Gtk::ScrolledWindow*>((entry_hbox->children().begin())->get_widget());
	Gtk::TextView *text_view_input = dynamic_cast<Gtk::TextView*>(*(scrolled_win2->get_children().begin()));
	
	BuddyListEntry *buddy_entry = client_->getClient()->getBuddyList()->getBuddyById(intify(label_id->get_text()));
	
	if (!buddy_entry->isOnline())
		;// if user is offline do nothing
	else {
		if (client_->sendMessage(buddy_entry->username, text_view_input->get_buffer()->get_text()) == true)
			updateTextView(text_view, client_->getUsername(), text_view_input->get_buffer()->get_text());
		text_view_input->get_buffer()->set_text("");
	}
}

void ConvoWin::updateTextView(Gtk::TextView *text_view, Glib::ustring nickname, Glib::ustring text) {
	Glib::RefPtr<Gtk::TextBuffer> refBuffer = text_view->get_buffer();
	
	if (!refBuffer->get_tag_table()->lookup("foreground")) {
		Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch = refBuffer->create_tag("foreground");
		refTagMatch->property_foreground() = "dark grey";
	}
	
	//BuddyListEntry *buddy_entry = client_->getClient()->getBuddyList->getBuddyById(intify(label_id->get_text()))
	refBuffer->insert_with_tag(refBuffer->end(), "\n" + nickname + " says :", "foreground");
	refBuffer->insert(refBuffer->end(), "\n" + text);
// 	Glib::RefPtr<Gtk::TextBuffer::TagTable> refTagTable = Gtk::TextBuffer::TagTable::create();
// 	refTagTable->add(refTagMatch);
}

void ConvoWin::updatePage(Gtk::TreeModel::Row childrow) {
	ModelColumns m_Columns;
	
	Gtk::Notebook_Helpers::PageIterator i;
	Gtk::VBox *vbox;
	Gtk::Box_Helpers::BoxList::iterator bi;
	Gtk::Label *label_id;
	if (convonotebook_->get_n_pages()) {
		for(i = convonotebook_->pages().begin(); i != convonotebook_->pages().end(); i++) {
			vbox = dynamic_cast<Gtk::VBox*>(i->get_child());
			bi = vbox->children().begin();
			label_id = dynamic_cast<Gtk::Label*>(bi->get_widget());
			if (label_id->get_text() == stringify(childrow[m_Columns.m_col_id]))
				break;
		}
		Gtk::Label *label_nick = dynamic_cast<Gtk::Label*>((++bi)->get_widget());
		Gtk::Label *label = dynamic_cast<Gtk::Label*>((++bi)->get_widget());
		Gtk::ScrolledWindow *scrolled_win = dynamic_cast<Gtk::ScrolledWindow*>((++bi)->get_widget());
		Gtk::HBox *entry_hbox = dynamic_cast<Gtk::HBox*>((++bi)->get_widget());
		Gtk::ScrolledWindow *scrolled_win2 = dynamic_cast<Gtk::ScrolledWindow*>((entry_hbox->children().begin())->get_widget());
		Gtk::TextView *text_view2 = dynamic_cast<Gtk::TextView*>(*(scrolled_win2->get_children().begin()));
		label_nick->set_text(childrow[m_Columns.m_col_nickname]);
		label->set_text("To: " + childrow[m_Columns.m_col_nickname] + " <" + childrow[m_Columns.m_col_status] + '>');
		if (childrow[m_Columns.m_col_status] == "Offline") {
			text_view2->get_buffer()->set_text("The user is currently offline");
			text_view2->set_sensitive(false);
		}
		else {
			text_view2->get_buffer()->set_text("");
			text_view2->set_sensitive(true);
		}
	}
	
}

void ConvoWin::onVScrollValueChange(Gtk::ScrolledWindow *scrolled_win) {
	scrolled_win->get_vscrollbar()->set_value(scrolled_win->get_vscrollbar()->get_adjustment()->get_upper());
}

void ConvoWin::gotMessage(Gtk::TreeIter &iter, Glib::ustring message) {
	ModelColumns m_Columns;
	//app_ptr_->getBuddyListWin()->get_iter_at_username(username);
	/* check if a convo is already opened for the user */
	app_ptr_->appendPageConvoWin(iter);
	
	/* check if a convo is already opened for the user */
	Gtk::Notebook_Helpers::PageIterator i;
	for(i = convonotebook_->pages().begin(); i != convonotebook_->pages().end(); i++) {
		if (i->get_tab_label_text() == (*iter)[m_Columns.m_col_username])
			break;
	}
	Gtk::VBox *vbox = dynamic_cast<Gtk::VBox*>(i->get_child());
	Gtk::Box_Helpers::BoxList::iterator wi = vbox->children().begin();
	Gtk::Label *label_id = dynamic_cast<Gtk::Label*>(wi->get_widget());
	Gtk::Label *label_nick = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::Label *label = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::ScrolledWindow *scrolled_win = dynamic_cast<Gtk::ScrolledWindow*>((++wi)->get_widget());
	Gtk::TextView *text_view = dynamic_cast<Gtk::TextView*>(*(scrolled_win->get_children().begin()));
	updateTextView(text_view, label_nick->get_text(), message);
}

