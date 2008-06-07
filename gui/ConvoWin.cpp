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
	
	cout << "lalalalalalala" << endl;
	
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
			if (stringify((*iter)[m_Columns.m_col_id]) == label_i->get_text())
				return 0;
		}
	}
	
	/* create the new child widget */
	Gtk::VBox *notebook_vbox = new Gtk::VBox();
	Gtk::HBox *entry_hbox = new Gtk::HBox();
	Gtk::Button *entry_btn = new Gtk::Button("Send");
	Gtk::Label *label = new Gtk::Label("To: " + (*iter)[m_Columns.m_col_nickname]);
	Gtk::Label *label_id = new Gtk::Label();
	label_id->set_label(stringify((*iter)[m_Columns.m_col_id]));
	Gtk::TextView *text_view = new Gtk::TextView();
	text_view->set_editable(false);
	Gtk::Entry *entry = new Gtk::Entry();
	notebook_vbox->pack_start(*label_id, false, false, 0);
	notebook_vbox->pack_start(*label, false, false, 0);
	notebook_vbox->pack_start(*text_view, true, true, 0);
	notebook_vbox->pack_start(*entry_hbox, false, false, 0);
	entry_hbox->pack_start(*entry, true, true, 0);
	entry_hbox->pack_start(*entry_btn, true, true, 0);
	
	/* add it to the notebook */
	convonotebook_->append_page(*notebook_vbox, (*iter)[m_Columns.m_col_username]);
	convonotebook_->show();
	notebook_vbox->show();
	label->show();
	text_view->show();
	entry->show();
	entry_hbox->show();
	entry_btn->show();
	
	/* add the button signal */
	entry_btn->signal_pressed().connect(sigc::bind<1>(sigc::mem_fun(*this, &ConvoWin::onSendButtonPressed), notebook_vbox));
	
	return 1;
}

void ConvoWin::onSendButtonPressed(Gtk::VBox *vbox) {
	cout << "YAAY BUTTON HAS BEEN PWESSEEEEDDD" << endl;
	Gtk::Box_Helpers::BoxList::iterator wi = vbox->children().begin();
	Gtk::Label *label_id = dynamic_cast<Gtk::Label*>(wi->get_widget());
	Gtk::Label *label = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::TextView *text_view = dynamic_cast<Gtk::TextView*>((++wi)->get_widget());
	Gtk::HBox *entry_hbox = dynamic_cast<Gtk::HBox*>((++wi)->get_widget());
	Gtk::Entry *entry = dynamic_cast<Gtk::Entry*>((entry_hbox->children().begin())->get_widget());
	
	Glib::RefPtr<Gtk::TextBuffer> previous = text_view->get_buffer();
	//Gtk::TextBuffer *buffer = new Gtk::TextBuffer(previous);
}




