#include <iostream>
#include <stdexcept>
#include <string>

#include "ConvoWin.h"

#include <libglademm/xml.h>
#include <gtkmm.h>
#include "../ModelColumns.h"

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

void ConvoWin::appendPage(Gtk::TreeModel::iterator &iter) {
	ModelColumns m_Columns;
	
	Gtk::VBox *notebook_vbox = new Gtk::VBox();
	Gtk::Label *label = new Gtk::Label();
	Gtk::Label *label_id = new Gtk::Label();
	Gtk::TextView *text_view = new Gtk::TextView();
	Gtk::Entry *entry = new Gtk::Entry();
	notebook_vbox->pack_start(*label_id, false, false, 0);
	notebook_vbox->pack_start(*label, false, false, 0);
	notebook_vbox->pack_start(*text_view, true, true, 0);
	notebook_vbox->pack_start(*entry, false, false, 0);
	
	convonotebook_->append_page(*notebook_vbox, (*iter)[m_Columns.m_col_username]);
	convonotebook_->show();
	notebook_vbox->show();
	label->show();
	text_view->show();
	entry->show();
	
	
}

