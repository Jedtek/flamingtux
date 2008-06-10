/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is GPL, licensing soon though ;) */
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <ctime>

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
	: convowin_(0), windowMinimized(0), windowAbove(0) {
	app_ptr_ = app;
	client_ = client;
	refXml_ = refXml;
	
	refXml->get_widget("ConvoWin", convowin_);
	if (!convowin_)
		throw std::runtime_error("Couldn't find ConvoWin");	
	refXml->get_widget("ConvoWinVBox", convowinvbox_);
	if (!convowinvbox_)
		throw std::runtime_error("Couldn't find ConvoWinVBox");
	
	convowin_->signal_delete_event().connect(sigc::mem_fun(*this, &ConvoWin::onDeleteEvent));
	convowin_->signal_window_state_event().connect_notify(sigc::mem_fun(*this, &ConvoWin::onWindowStateEvent));
	
	convonotebook_ = new Gtk::Notebook();
	convowinvbox_->pack_end(*convonotebook_, true, true, 0);
	convonotebook_->signal_page_removed().connect_notify(sigc::mem_fun(*this, &ConvoWin::onPageRemoved));
	convonotebook_->signal_switch_page().connect_notify(sigc::mem_fun(*this, &ConvoWin::onPageSwitched));
	convonotebook_->set_scrollable(true);
}

ConvoWin::~ConvoWin() {
	
}

void ConvoWin::onWindowStateEvent(GdkEventWindowState* event) {
	if (event->new_window_state == GDK_WINDOW_STATE_ICONIFIED)
		windowMinimized = 1;
	else {
		windowMinimized = 0;
		convowin_->set_urgency_hint(false);
	}
}

bool ConvoWin::onDeleteEvent(GdkEventAny *e) {
	
	Gtk::MessageDialog close_dialog(*this, "Close tabs", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
	close_dialog.set_secondary_text("Are you sure you want to close all opened tabs?");
	int d_result = close_dialog.run();
	switch(d_result) {
		case(Gtk::RESPONSE_YES): {
			int i;
			Gtk::VBox *vbox;
			for(i = 0; i != convonotebook_->get_n_pages(); ) {
				vbox = (Gtk::VBox *) (convonotebook_->get_nth_page(i));
				closeTab(vbox);
			}
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

void ConvoWin::onPageRemoved(Gtk::Widget* page, guint page_num) {
	if (!convonotebook_->get_n_pages())
		convowin_->hide();
}

void ConvoWin::onPageSwitched(GtkNotebookPage* page, guint page_num) {
	Gtk::HBox *hbox = (Gtk::HBox *) convonotebook_->get_tab_label(*(convonotebook_->get_nth_page(page_num)));	
	Gtk::Label *label = dynamic_cast<Gtk::Label *>((hbox->children().begin())->get_widget());
	//Gtk::Label *label = dynamic_cast<Gtk::Label *>(*(eb->get_children().begin()));
	label->set_markup(label->get_text());
	convowin_->set_urgency_hint(false);
	convowin_->set_title(label->get_text());
}

Gtk::Notebook_Helpers::PageIterator ConvoWin::appendPage(Gtk::TreeModel::iterator &iter) {
	ModelColumns m_Columns;
	
	cout << (*iter)[m_Columns.m_col_id] << endl;
	/* check if a convo is already opened for the user */
	Gtk::Notebook_Helpers::PageIterator i = convonotebook_->pages().begin();
	if (convonotebook_->get_n_pages()) {
		Gtk::VBox *vbox;
		Gtk::Box_Helpers::BoxList::iterator bi;
		Gtk::Label *label_i;
		for(i = convonotebook_->pages().begin(); i != convonotebook_->pages().end(); i++) {
			vbox = dynamic_cast<Gtk::VBox*>(i->get_child());
			bi = vbox->children().begin();
			label_i = dynamic_cast<Gtk::Label*>(bi->get_widget());
			if (stringify((*iter)[m_Columns.m_col_id]) == label_i->get_text()) {
				//convonotebook_->set_current_page(convonotebook_->page_num(*vbox));
				return i;
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
	
	/* create the page label with close button */
	Gtk::HBox *page_hbox = Gtk::manage(new Gtk::HBox);
	Gtk::Label *page_label = Gtk::manage(new Gtk::Label());
	page_label->set_markup((*iter)[m_Columns.m_col_username]);
	//Gtk::EventBox *page_eb = Gtk::manage(new Gtk::EventBox);
	//page_eb->add(*page_label);
	page_hbox->pack_start(*page_label);
	Gtk::Button *page_button = Gtk::manage(new Gtk::Button);
	Gtk::Image *image = Gtk::manage(new Gtk::Image(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_MENU)); 
	page_button->add(*image); 
	page_button->signal_clicked().
	connect(sigc::bind<Gtk::VBox*>(sigc::mem_fun(*this, &ConvoWin::onCloseTabClicked), notebook_vbox) ); 
	page_hbox->pack_start(*page_button, Gtk::PACK_SHRINK);
	
	scrolled_win->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	scrolled_win->add(*text_view);
	scrolled_win2->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	scrolled_win2->add(*text_view2);
	notebook_vbox->pack_start(*label_id, false, false, 0);
	notebook_vbox->pack_start(*label_nick, false, false, 0);
	notebook_vbox->pack_start(*label, false, false, 0);
	notebook_vbox->pack_start(*scrolled_win, true, true, 0);
	notebook_vbox->pack_start(*entry_hbox, false, false, 5);
	entry_hbox->pack_start(*scrolled_win2, true, true, 0);
	entry_hbox->pack_start(*entry_btn, false, false, 0);
	
	/* add it to the notebook */
	convonotebook_->append_page(*notebook_vbox, *page_hbox);
	convonotebook_->show();
	page_hbox->show();
	page_hbox->show_all_children();
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
	text_view->set_cursor_visible(false);
	
	/* add the button signal */
	entry_btn->add_accelerator("activate", convowin_->get_accel_group(), GDK_Return, 
				   (Gdk::ModifierType) 0, Gtk::ACCEL_VISIBLE);
	entry_btn->signal_pressed().connect(sigc::bind(sigc::mem_fun(*this, &ConvoWin::onSendButtonPressed), notebook_vbox, iter));
	entry_btn->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &ConvoWin::onSendButtonPressed), notebook_vbox, iter));
	
	//convonotebook_->set_current_page(convonotebook_->page_num(*notebook_vbox));
	
	scrolled_win->get_vscrollbar()->get_adjustment()->signal_changed().connect(sigc::bind(mem_fun(*this,&ConvoWin::onVScrollValueChange), scrolled_win));
				 
	return i;
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
	
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [15];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strftime (buffer, 15, "[%I:%M:%S %p]", timeinfo);
	
	string x(buffer);
	//BuddyListEntry *buddy_entry = client_->getClient()->getBuddyList->getBuddyById(intify(label_id->get_text()))
	refBuffer->insert_with_tag(refBuffer->end(), '\n' + x + ' ' + nickname + " says :", "foreground");
	refBuffer->insert(refBuffer->end(), "\n" + text);
// 	Glib::RefPtr<Gtk::TextBuffer::TagTable> refTagTable = Gtk::TextBuffer::TagTable::create();
// 	refTagTable->add(refTagMatch);
}

void ConvoWin::updateStatusTextView(Gtk::TextView *text_view, Glib::ustring nickname, int status) {
	Glib::RefPtr<Gtk::TextBuffer> refBuffer = text_view->get_buffer();
	
	if (!refBuffer->get_tag_table()->lookup("status")) {
		Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch = refBuffer->create_tag("status");
		refTagMatch->property_foreground() = "dark red";
		refTagMatch->property_style() = Pango::STYLE_OBLIQUE;
	}
	if (status)
		refBuffer->insert_with_tag(refBuffer->end(), "\n<" + nickname + "is now Online>", "status");
	else
		refBuffer->insert_with_tag(refBuffer->end(), "\n<" + nickname + "is now Offline>", "status");
}

void ConvoWin::updatePage(Gtk::TreeModel::Row childrow) {
	ModelColumns m_Columns;
	
	Gtk::Notebook_Helpers::PageIterator i;
	Gtk::Box_Helpers::BoxList::iterator bi;
	Gtk::VBox *vbox;
	Gtk::Label *label_id;
	if (convonotebook_->get_n_pages()) {
		for (i = convonotebook_->pages().begin(); i != convonotebook_->pages().end(); i++) {
			vbox = dynamic_cast<Gtk::VBox*>(i->get_child());
			bi = vbox->children().begin();;
			label_id = dynamic_cast<Gtk::Label*>(bi->get_widget());
			if (intify(label_id->get_text()) == childrow[m_Columns.m_col_id]) {
				break;
			}
		}
		if (i == convonotebook_->pages().end())
			;// do nothing
		else {
			Gtk::Label *label_nick = dynamic_cast<Gtk::Label*>((++bi)->get_widget());
			Gtk::Label *label = dynamic_cast<Gtk::Label*>((++bi)->get_widget());
			Gtk::ScrolledWindow *scrolled_win = dynamic_cast<Gtk::ScrolledWindow*>((++bi)->get_widget());
			Gtk::TextView *text_view = dynamic_cast<Gtk::TextView*>(*(scrolled_win->get_children().begin()));
			Gtk::HBox *entry_hbox = dynamic_cast<Gtk::HBox*>((++bi)->get_widget());
			Gtk::ScrolledWindow *scrolled_win2 = dynamic_cast<Gtk::ScrolledWindow*>((entry_hbox->children().begin())->get_widget());
			Gtk::TextView *text_view2 = dynamic_cast<Gtk::TextView*>(*(scrolled_win2->get_children().begin()));
			label_nick->set_text(childrow[m_Columns.m_col_nickname]);
			label->set_text("To: " + childrow[m_Columns.m_col_nickname] + " <" + childrow[m_Columns.m_col_status] + '>');
			if (childrow[m_Columns.m_col_status] == "Offline") {
				//updateStatusTextView(text_view, childrow[m_Columns.m_col_nickname], 0);
				text_view2->get_buffer()->set_text("The user is currently offline");
				text_view2->set_sensitive(false);
			}
			else {
				//updateStatusTextView(text_view, childrow[m_Columns.m_col_nickname], 1);
				text_view2->get_buffer()->set_text("");
				text_view2->set_sensitive(true);
			}
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
	Gtk::Notebook_Helpers::PageIterator i = app_ptr_->appendPageConvoWin(iter);
// 	if (!i)
// 		i = app_ptr_->appendPageConvoWin(iter);
// 	/* check if a convo is already opened for the user */
// 	Gtk::Notebook_Helpers::PageIterator i;
// 	for(i = convonotebook_->pages().begin(); i != convonotebook_->pages().end(); i++) {
// 		cout << "LALALALALALALALALLA: " << (i->get_tab_label_text() << endl;
// 		if (i->get_tab_label_text() == (*iter)[m_Columns.m_col_username])
// 			break;
// 	}
	
	Gtk::VBox *vbox = dynamic_cast<Gtk::VBox*>(i->get_child());
	Gtk::Box_Helpers::BoxList::iterator wi = vbox->children().begin();
	Gtk::Label *label_id = dynamic_cast<Gtk::Label*>(wi->get_widget());
	Gtk::Label *label_nick = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::Label *label = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::ScrolledWindow *scrolled_win = dynamic_cast<Gtk::ScrolledWindow*>((++wi)->get_widget());
	Gtk::TextView *text_view = dynamic_cast<Gtk::TextView*>(*(scrolled_win->get_children().begin()));
	updateTextView(text_view, label_nick->get_text(), message);
	
	if (convonotebook_->get_current_page() != convonotebook_->page_num(*vbox)) {
		Gtk::HBox *hbox = (Gtk::HBox *) convonotebook_->get_tab_label(*vbox);	
		Gtk::Label *label = dynamic_cast<Gtk::Label *>((hbox->children().begin())->get_widget());
// 		Gtk::Label *label = dynamic_cast<Gtk::Label *>(*(eb->get_children().begin()));
		label->set_markup("<b><span color='Red'>" + label->get_text() + "</span></b>");
		convowin_->set_urgency_hint(true);
	}
	if (windowMinimized) {
		convowin_->set_urgency_hint(true);
	}
}

void ConvoWin::onCloseTabClicked(Gtk::VBox *notebook_vbox) {
	closeTab(notebook_vbox);
}

void ConvoWin::closeTab(Gtk::VBox *notebook_vbox) {
	
	convonotebook_->remove_page(*notebook_vbox);
		
	Gtk::Box_Helpers::BoxList::iterator wi = notebook_vbox->children().begin();
	Gtk::Label *label_id = dynamic_cast<Gtk::Label*>(wi->get_widget());
	Gtk::Label *label_nick = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::Label *label = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::ScrolledWindow *scrolled_win = dynamic_cast<Gtk::ScrolledWindow*>((++wi)->get_widget());
	Gtk::TextView *text_view = dynamic_cast<Gtk::TextView*>(*(scrolled_win->get_children().begin()));
	Gtk::HBox *entry_hbox = dynamic_cast<Gtk::HBox*>((++wi)->get_widget());
	Gtk::ScrolledWindow *scrolled_win2 = dynamic_cast<Gtk::ScrolledWindow*>((entry_hbox->children().begin())->get_widget());
	Gtk::Button *send_btn = dynamic_cast<Gtk::Button*>((++(entry_hbox->children().begin()))->get_widget());
	Gtk::TextView *text_view_input = dynamic_cast<Gtk::TextView*>(*(scrolled_win2->get_children().begin()));
		
	delete text_view_input;
	delete send_btn;
	delete scrolled_win2;
	delete entry_hbox;
	delete text_view;
	delete scrolled_win;
	delete label;
	delete label_nick;
	delete label_id;
	delete notebook_vbox;
	
}
