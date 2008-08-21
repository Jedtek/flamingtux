/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 *  This code is protected by GPLv3, please see LICENSE file for more information */
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <ctime>

#include "ConvoWin.h"

#include <libglademm/xml.h>
#include <gtkmm.h>
#include <gtk/gtk.h>
#include "../Application.h"
#include "PreferencesWin.h"
#include "../ModelColumns.h"
#include "../CommonFunctions.h"
#include "../backend/GtkHelperFunctions.h"

using namespace std;
using namespace xfirelib;
using namespace xfireclient;

ConvoWin::ConvoWin(Glib::RefPtr<Gnome::Glade::Xml> refXml, Application *app, FireClient *client)
	: convowin_(0), windowMinimized(0), windowAbove(0), convonotebook_(0) {
	app_ptr_ = app;
	client_ = client;
	refXml_ = refXml;
	
	refXml->get_widget("ConvoWin", convowin_);
	if (!convowin_)
		throw std::runtime_error("Couldn't find ConvoWin");	
	refXml->get_widget("ConvoWinVBox", convowinvbox_);
	if (!convowinvbox_)
		throw std::runtime_error("Couldn't find ConvoWinVBox");
	refXml->get_widget("ConvoWinCloseMenuItem", close_widget_);
	if (!close_widget_)
		throw std::runtime_error("Couldn't find ConvoWinCloseMenuItem");
	
	close_widget_->signal_activate().connect(sigc::mem_fun(*this, &ConvoWin::onCloseMenuItem));
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

void ConvoWin::onCloseMenuItem() {
	onDeleteEvent(NULL);	
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
	
	Gtk::MessageDialog *close_dialog = new Gtk::MessageDialog(*this, "Close tabs", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
	close_dialog->set_secondary_text("Are you sure you want to close all opened tabs?");
	close_dialog->signal_response().connect(sigc::bind(sigc::mem_fun(*this, &ConvoWin::onCloseDialogResponseSignal), close_dialog));
	close_dialog->show();
	return true;
}

void ConvoWin::onCloseDialogResponseSignal(int response_id, Gtk::MessageDialog *dialog) {
	switch(response_id) {
		case(Gtk::RESPONSE_YES): {
			int i;
			Gtk::VBox *vbox;
			for(i = 0; i != convonotebook_->get_n_pages(); ) {
				vbox = (Gtk::VBox *) (convonotebook_->get_nth_page(i));
				closeTab(vbox);
			}
			convowin_->hide();
			break;
		}
		case(Gtk::RESPONSE_NONE):
		case(Gtk::RESPONSE_NO): {
			/* Do nothing and return true */
			break;
		}
	}
	dialog->hide();
	delete dialog;
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
	convowin_->set_title(label->get_text() + " - Chat");
}

Gtk::Notebook_Helpers::PageIterator ConvoWin::appendPage(Gtk::TreeModel::iterator &iter) {
	ModelColumns m_Columns;

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
	Gtk::EventBox *scrolled_win_eb = new Gtk::EventBox();
	scrolled_win_eb->modify_bg(Gtk::STATE_NORMAL, Gdk::Color("dark grey"));
	Gtk::EventBox *entry_hbox_eb = new Gtk::EventBox();
	entry_hbox_eb->modify_bg(Gtk::STATE_NORMAL, Gdk::Color("dark grey"));
	Gtk::VBox *notebook_vbox = new Gtk::VBox();
	Gtk::HBox *entry_hbox = new Gtk::HBox();
	entry_hbox_eb->add(*entry_hbox);
	entry_hbox->set_border_width(1);
	Gtk::Button *entry_btn = new Gtk::Button("Send");
	Gtk::Label *label = new Gtk::Label();
	Glib::ustring nickname_copy((*iter)[m_Columns.m_col_nickname]);
	nickname_copy = parseMarkup(nickname_copy);
	label->set_markup("To: " + nickname_copy + " &lt;" + (*iter)[m_Columns.m_col_status] + "&gt;");
	Gtk::Label *label_id = new Gtk::Label();
	Gtk::Label *label_nick = new Gtk::Label((*iter)[m_Columns.m_col_nickname]);
	label_id->set_label(stringify((*iter)[m_Columns.m_col_id]));
	Gtk::ScrolledWindow *scrolled_win = new Gtk::ScrolledWindow();
	scrolled_win_eb->add(*scrolled_win);
	scrolled_win->set_border_width(1);
	Gtk::ScrolledWindow *scrolled_win2 = new Gtk::ScrolledWindow();
	//scrolled_win2->set_border_width(1);
	Gtk::TextView *text_view = new Gtk::TextView();
	text_view->set_editable(false);
	Gtk::TextView *text_view2 = new Gtk::TextView();
	attachGtkSpellToTextView(text_view2);
	
	Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch = text_view2->get_buffer()->create_tag("inputtag");
	refTagMatch->property_font() = app_ptr_->getConfig()->getConfigOptions()->getFont();
	refTagMatch->property_foreground() = app_ptr_->getConfig()->getConfigOptions()->getColor();
	/* create the page label with close button */
	Gtk::VPaned *vpaned = Gtk::manage(new Gtk::VPaned);
	Gtk::VBox *entry_vbox = Gtk::manage(new Gtk::VBox);
	Gtk::HBox *page_hbox = Gtk::manage(new Gtk::HBox);
	Gtk::HBox *buttons_hbox = Gtk::manage(new Gtk::HBox);
	Gtk::Alignment *entry_alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT, Gtk::ALIGN_CENTER, 0.0, 
			0.0));
	Gtk::EventBox *entry_eb = Gtk::manage(new Gtk::EventBox);
	Gtk::Label *page_label = Gtk::manage(new Gtk::Label());
	page_label->set_markup((*iter)[m_Columns.m_col_username]);
	//Gtk::EventBox *page_eb = Gtk::manage(new Gtk::EventBox);
	//page_eb->add(*page_label);
	page_hbox->pack_start(*page_label);
	Gtk::Button *page_button = Gtk::manage(new Gtk::Button);
	Gtk::Image *image = Gtk::manage(new Gtk::Image(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_MENU)); 
	page_button->add(*image); 
	page_button->signal_clicked().connect(sigc::bind<Gtk::VBox*>(sigc::mem_fun(*this, &ConvoWin::onCloseTabClicked), notebook_vbox) );
	page_button->add_accelerator("clicked", convowin_->get_accel_group(), 'w', 
				   Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
	page_hbox->pack_start(*page_button, Gtk::PACK_SHRINK);
	
	scrolled_win->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	scrolled_win->add(*text_view);
	scrolled_win2->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	scrolled_win2->add(*text_view2);
	text_view->set_wrap_mode(Gtk::WRAP_WORD_CHAR);
	text_view2->set_wrap_mode(Gtk::WRAP_WORD_CHAR);
	notebook_vbox->pack_start(*label_id, false, false, 0);
	notebook_vbox->pack_start(*label_nick, false, false, 0);
	notebook_vbox->pack_start(*label, false, false, 0);
	notebook_vbox->pack_start(*vpaned, true, true, 0);
// 	notebook_vbox->pack_start(*scrolled_win, true, true, 0);
// 	notebook_vbox->pack_start(*entry_hbox, false, false, 5);
	vpaned->pack1(*scrolled_win_eb);
	vpaned->pack2(*entry_vbox);
	entry_vbox->pack_start(*buttons_hbox, false, false, 0);
	entry_vbox->pack_start(*entry_hbox_eb, true, true, 0);
	entry_hbox->pack_start(*scrolled_win2, true, true, 0);
	entry_hbox->pack_start(*entry_eb, false, false, 0);
	entry_eb->add(*entry_alignment);
	entry_alignment->add(*entry_btn);
	entry_eb->modify_bg(Gtk::STATE_NORMAL, Gdk::Color("white"));
	entry_btn->set_size_request(80, 30);
	vpaned->set_position(250);
	// create the buttons on the buttons_hbox
	Gtk::Button *font_btn = Gtk::manage(new Gtk::Button);
	Gtk::Image *font_btn_image = Gtk::manage(new Gtk::Image(Gtk::Stock::SELECT_FONT,
			 Gtk::ICON_SIZE_LARGE_TOOLBAR)); 
	font_btn->add(*font_btn_image); 
	Gtk::Button *color_btn = Gtk::manage(new Gtk::Button);
	Gtk::Image *color_btn_image = Gtk::manage(new Gtk::Image(Gtk::Stock::COLOR_PICKER,
			 Gtk::ICON_SIZE_LARGE_TOOLBAR)); 
	color_btn->add(*color_btn_image); 
	Gtk::ToggleButton *underline_btn = Gtk::manage(new Gtk::ToggleButton);
	Gtk::Image *underline_btn_image = Gtk::manage(new Gtk::Image(Gtk::Stock::UNDERLINE,
			Gtk::ICON_SIZE_LARGE_TOOLBAR)); 
	underline_btn->add(*underline_btn_image);
	Gtk::ToggleButton *strikethrough_btn = Gtk::manage(new Gtk::ToggleButton);
	Gtk::Image *strikethrough_btn_image = Gtk::manage(new Gtk::Image(Gtk::Stock::STRIKETHROUGH,
			Gtk::ICON_SIZE_LARGE_TOOLBAR)); 
	strikethrough_btn->add(*strikethrough_btn_image);
	Gtk::ToggleButton *apply_btn = Gtk::manage(new Gtk::ToggleButton);
	Gtk::Image *apply_btn_image = Gtk::manage(new Gtk::Image(Gtk::Stock::APPLY,
			Gtk::ICON_SIZE_LARGE_TOOLBAR)); 
	apply_btn->add(*apply_btn_image);
	// pack them on the buttons_hbox
	buttons_hbox->pack_start(*font_btn, false, false, 0);
	font_btn->set_size_request(40, 40);
	buttons_hbox->pack_start(*color_btn, false, false, 0);
	color_btn->set_size_request(40, 40);
	buttons_hbox->pack_start(*underline_btn, false, false, 0);
	underline_btn->set_size_request(40, 40);
	buttons_hbox->pack_start(*strikethrough_btn, false, false, 0);
	strikethrough_btn->set_size_request(40, 40);
	buttons_hbox->pack_start(*apply_btn, false, false, 5);
	apply_btn->set_size_request(40, 40);
	
	/* add it to the notebook */
	convonotebook_->append_page(*notebook_vbox, *page_hbox);
	convonotebook_->show();
	page_hbox->show();
	page_hbox->show_all_children();
	notebook_vbox->show();
	label->show();
	vpaned->show();
	entry_vbox->show();
	buttons_hbox->show();
	font_btn->show();
	font_btn_image->show();
	color_btn->show();
	color_btn_image->show();
	underline_btn->show();
	underline_btn_image->show();
	strikethrough_btn->show();
	strikethrough_btn_image->show();
	apply_btn->show();
	apply_btn_image->show();
	scrolled_win_eb->show();
	entry_hbox_eb->show();
	scrolled_win->show();
	text_view->show();
	scrolled_win2->show();
	text_view2->show();
	entry_hbox->show();
	entry_eb->show();
	entry_alignment->show();
	entry_btn->show();
	
	if ((*iter)[m_Columns.m_col_status] == "Offline") {
		text_view2->get_buffer()->set_text("The user is currently offline");
		text_view2->set_sensitive(false);
	}
	text_view->set_cursor_visible(false);
	
	/* add the button signal */
	entry_btn->add_accelerator("activate", convowin_->get_accel_group(), GDK_Return, 
				   (Gdk::ModifierType) 0, Gtk::ACCEL_VISIBLE);
	entry_btn->signal_pressed().connect(sigc::bind(sigc::mem_fun(*this, &ConvoWin::onSendButtonPressed),
				   notebook_vbox, iter));
	entry_btn->signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &ConvoWin::onSendButtonPressed),
				    notebook_vbox, iter));
	font_btn->signal_clicked().connect(sigc::mem_fun(*this, &ConvoWin::onFontBtnClicked));
	color_btn->signal_clicked().connect(sigc::mem_fun(*this, &ConvoWin::onColorBtnClicked));
	underline_btn->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &ConvoWin::onUnderlineBtnClicked),
				      text_view2, underline_btn));
	strikethrough_btn->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, 
					  &ConvoWin::onStrikethroughBtnClicked),
				      text_view2, strikethrough_btn));
	apply_btn->signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &ConvoWin::onApplyBtnToggled), 
				  text_view2, buttons_hbox, apply_btn, underline_btn, strikethrough_btn));
	//convonotebook_->set_current_page(convonotebook_->page_num(*notebook_vbox));
	
	scrolled_win->get_vscrollbar()->get_adjustment()->signal_changed().connect(sigc::bind(mem_fun(*this,&ConvoWin::onVScrollValueChange), scrolled_win));
	text_view2->get_buffer()->signal_changed().connect(sigc::bind(mem_fun(*this,&ConvoWin::onTextBufferInputChanged), text_view2->get_buffer(), apply_btn));
	
	apply_btn->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getEnableStyle()));
	onApplyBtnToggled(text_view2, buttons_hbox, apply_btn, underline_btn, strikethrough_btn);
	refTagMatch->property_underline_set() = 0;
	refTagMatch->property_strikethrough_set() = 0;
				 
	return i;
}

void ConvoWin::attachGtkSpellToTextView(Gtk::TextView *view) {
	if (app_ptr_->getConfig()->getConfigOptions()->getSpellChecking() == "1") {
		GError *error = NULL;
		if (gtkspell_new_attach(view->gobj(), app_ptr_->getConfig()->getConfigOptions()->getSpellingDictionary().c_str(), &error) == NULL) {
			g_print("gtkspell error: %s\n", error->message);
			g_error_free(error);
		}
	}
}

void ConvoWin::setGtkSpellDictionary(GtkSpell *spell, Glib::ustring lang) {
	if (app_ptr_->getConfig()->getConfigOptions()->getSpellChecking() == "1") {
		GError *error = NULL;
		if (!gtkspell_set_language(spell, 
		    app_ptr_->getConfig()->getConfigOptions()->getSpellingDictionary().c_str(), &error)) {
			g_print("gtkspell error: %s\n", error->message);
			g_error_free(error);
		}
	}
}

void ConvoWin::onUnderlineBtnClicked(Gtk::TextView *text_view_input, Gtk::ToggleButton *underline_btn) {
	Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch =
			text_view_input->get_buffer()->get_tag_table()->lookup("inputtag");
	if (underline_btn->get_active())
		refTagMatch->property_underline() = Pango::UNDERLINE_SINGLE;
	else
		refTagMatch->property_underline() = Pango::UNDERLINE_NONE;
	refTagMatch->property_underline_set() = underline_btn->get_active();
}

void ConvoWin::onStrikethroughBtnClicked(Gtk::TextView *text_view_input, Gtk::ToggleButton *strikethrough_btn) {
	Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch =
			text_view_input->get_buffer()->get_tag_table()->lookup("inputtag");
	refTagMatch->property_strikethrough() = strikethrough_btn->get_active();
	refTagMatch->property_strikethrough_set() = strikethrough_btn->get_active();
}

void ConvoWin::onFontBtnClicked() {
	app_ptr_->getBuddyListWin()->getPreferencesWin()->signalFontBtnClicked();
}

void ConvoWin::onColorBtnClicked() {
	app_ptr_->getBuddyListWin()->getPreferencesWin()->signalColorBtnClicked();
}

void ConvoWin::onForeachButtonsHBox(Gtk::Widget &widget, int sensitive) {
	widget.set_sensitive(sensitive);
}

void ConvoWin::onApplyBtnToggled(Gtk::TextView *text_view_input, Gtk::HBox *buttons_hbox, Gtk::ToggleButton *apply_btn, Gtk::ToggleButton *underline_btn, Gtk::ToggleButton *strikethrough_btn) {
	buttons_hbox->foreach(sigc::bind(sigc::mem_fun(*this, &ConvoWin::onForeachButtonsHBox), 
			      apply_btn->get_active()));
	apply_btn->set_sensitive(1);
	Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch =
			text_view_input->get_buffer()->get_tag_table()->lookup("inputtag");
 	refTagMatch->property_underline_set() = underline_btn->get_active();
 	refTagMatch->property_strikethrough_set() = strikethrough_btn->get_active();
	if (!apply_btn->get_active())
		refTagMatch->property_font() = "";
	else
		refTagMatch->property_font() = app_ptr_->getConfig()->getConfigOptions()->getFont();
	
	updateTextViewInputStyle(text_view_input->get_buffer(), apply_btn);
}

void ConvoWin::onTextBufferInputChanged(Glib::RefPtr<Gtk::TextBuffer> buffer, Gtk::ToggleButton *apply_btn) {
	Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch =
			buffer->get_tag_table()->lookup("inputtag");
	if (app_ptr_->getConfig()->getConfigOptions()->getEnableStyle() == "1" && apply_btn->get_active())
		buffer->apply_tag_by_name("inputtag", buffer->begin(), buffer->end());
}

Gtk::TextView *ConvoWin::getTextViewInput(Gtk::VBox *vbox) {
	Gtk::Box_Helpers::BoxList::iterator wi = vbox->children().begin();
	Gtk::Label *label_id = dynamic_cast<Gtk::Label*>(wi->get_widget());
	Gtk::Label *label_nick = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::Label *label = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::VPaned *vpaned = dynamic_cast<Gtk::VPaned*>((++wi)->get_widget());
	Gtk::VBox *entry_vbox = dynamic_cast<Gtk::VBox*>(vpaned->get_child2());
	Gtk::Box_Helpers::BoxList::iterator wi2 = entry_vbox->children().begin();
	Gtk::EventBox *entry_hbox_eb = dynamic_cast<Gtk::EventBox*>((++wi2)->get_widget());
	Gtk::HBox *entry_hbox = dynamic_cast<Gtk::HBox*>(entry_hbox_eb->get_child());
	Gtk::ScrolledWindow *scrolled_win2 = dynamic_cast<Gtk::ScrolledWindow*>((entry_hbox->children().begin())->get_widget());
	Gtk::TextView *text_view_input = dynamic_cast<Gtk::TextView*>(scrolled_win2->get_child());
	
	return text_view_input;
}

Gtk::TextView *ConvoWin::getTextView(Gtk::VBox *vbox) {
	Gtk::Box_Helpers::BoxList::iterator wi = vbox->children().begin();
	Gtk::Label *label_id = dynamic_cast<Gtk::Label*>(wi->get_widget());
	Gtk::Label *label_nick = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::Label *label = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::VPaned *vpaned = dynamic_cast<Gtk::VPaned*>((++wi)->get_widget());
	Gtk::EventBox *scrolled_win_eb = dynamic_cast<Gtk::EventBox*>(vpaned->get_child1());
	Gtk::ScrolledWindow *scrolled_win = dynamic_cast<Gtk::ScrolledWindow*>(scrolled_win_eb->get_child());
	Gtk::TextView *text_view = dynamic_cast<Gtk::TextView*>(scrolled_win->get_child());
	
	return text_view;
}

Gtk::ToggleButton *ConvoWin::getApplyBtn(Gtk::VBox *vbox) {
	Gtk::Box_Helpers::BoxList::iterator wi = vbox->children().begin();
	Gtk::Label *label_id = dynamic_cast<Gtk::Label*>(wi->get_widget());
	Gtk::Label *label_nick = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::Label *label = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::VPaned *vpaned = dynamic_cast<Gtk::VPaned*>((++wi)->get_widget());
	Gtk::VBox *entry_vbox = dynamic_cast<Gtk::VBox*>(vpaned->get_child2());
	Gtk::HBox *buttons_hbox = dynamic_cast<Gtk::HBox*>((entry_vbox->children().begin())->get_widget());
	Gtk::Box_Helpers::BoxList::iterator wi2 = buttons_hbox->children().end();
	--wi2;
	Gtk::ToggleButton *apply_btn = dynamic_cast<Gtk::ToggleButton*>(wi2->get_widget());
	return apply_btn;
}

Gtk::Label *ConvoWin::getLabelId(Gtk::VBox *vbox) {
	Gtk::Box_Helpers::BoxList::iterator wi = vbox->children().begin();
	Gtk::Label *label_id = dynamic_cast<Gtk::Label*>(wi->get_widget());
	
	return label_id;
}

Gtk::Label *ConvoWin::getLabelNick(Gtk::VBox *vbox) {
	Gtk::Box_Helpers::BoxList::iterator wi = vbox->children().begin();
	Gtk::Label *label_id = dynamic_cast<Gtk::Label*>(wi->get_widget());
	Gtk::Label *label_nick = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	
	return label_nick;
}

Gtk::Label *ConvoWin::getLabel(Gtk::VBox *vbox) {
	Gtk::Box_Helpers::BoxList::iterator wi = vbox->children().begin();
	Gtk::Label *label_id = dynamic_cast<Gtk::Label*>(wi->get_widget());
	Gtk::Label *label_nick = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	Gtk::Label *label = dynamic_cast<Gtk::Label*>((++wi)->get_widget());
	
	return label;
}

void ConvoWin::onSendButtonPressed(Gtk::VBox *vbox, Gtk::TreeModel::iterator &iter) {
	ModelColumns m_Columns;
	
	Gtk::Label *label_id = getLabelId(vbox);
	Gtk::TextView *text_view = getTextView(vbox);
	Gtk::TextView *text_view_input = getTextViewInput(vbox);
	
	BuddyListEntry *buddy_entry = client_->getClient()->getBuddyList()->getBuddyById(intify(label_id->get_text()));
	
	if (!buddy_entry->isOnline())
		;// if user is offline do nothing
	else {
		if (text_view_input->get_buffer()->get_text() == "")
			;// do nothing
		else {
			Glib::ustring text = text_view_input->get_buffer()->get_text();
			Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch =
					text_view_input->get_buffer()->get_tag_table()->lookup("inputtag");
			if (app_ptr_->getConfig()->getConfigOptions()->getEnableStyle() == "1" && 
						 getApplyBtn(vbox)->get_active()) {
				
				text = parseMarkup(text);
				Glib::ustring span_tmp("<span font_desc='" + app_ptr_->getConfig()->getConfigOptions()->getFont() + "'><span foreground='" + app_ptr_->getConfig()->getConfigOptions()->getColor() + "'>");
				text.insert(0, span_tmp);
				text.append("</span></span>");
				if (refTagMatch->property_underline_set()) {
					text.insert(0, "<u>");
					text.append("</u>");
				}
				if (refTagMatch->property_strikethrough_set()) {
					text.insert(0, "<s>");
					text.append("</s>");
				}
			}
			if (client_->sendMessage(buddy_entry->username, text) == true) {
				Glib::ustring nickname = app_ptr_->getConfig()->getConfigOptions()->getNickname();
				if (nickname == "")
					nickname = "[" + client_->getUsername() + "]";
				updateTextView(text_view, nickname, text);
				app_ptr_->getLog()->chatLogSend(buddy_entry->username, text, nickname);
			}
		}
		text_view_input->get_buffer()->set_text("");
	}
}

void ConvoWin::updateTextView(Gtk::TextView *text_view, Glib::ustring nickname, Glib::ustring text) {
	Glib::RefPtr<Gtk::TextBuffer> refBuffer = text_view->get_buffer();
	
	Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch;
	if (!refBuffer->get_tag_table()->lookup("fforeground")) {
		Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch = refBuffer->create_tag("fforeground");
		refTagMatch->property_foreground() = "dark grey";
	}
	
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [15];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strftime (buffer, 15, "[%I:%M:%S %p]", timeinfo);
	
	Glib::ustring buffer_copy(buffer);
	
	Glib::ustring nickname_copy = nickname;
	nickname_copy = parseMarkup(nickname_copy);
	//BuddyListEntry *buddy_entry = client_->getClient()->getBuddyList->getBuddyById(intify(label_id->get_text()))
	refBuffer->insert_with_tag(refBuffer->end(), '\n' + buffer_copy + ' ', "fforeground");
	gtkTextBufferInsertMarkupWithTag(refBuffer, refBuffer->end(), nickname_copy, "fforeground");
	refBuffer->insert_with_tag(refBuffer->end(), " says :", "fforeground");
	refBuffer->insert(refBuffer->end(), "\n");
	gtkTextBufferInsertMarkup(refBuffer, refBuffer->end(), text);
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
			label_id = getLabelId(vbox);
			if (intify(label_id->get_text()) == childrow[m_Columns.m_col_id]) {
				break;
			}
		}
		if (i == convonotebook_->pages().end())
			;// do nothing
		else {
			Gtk::Label *label_nick = getLabelNick(vbox);
			Gtk::Label *label = getLabel(vbox);
			Gtk::TextView *text_view2 = getTextViewInput(vbox);
			label_nick->set_text(childrow[m_Columns.m_col_nickname]);
			Glib::ustring nickname_copy(childrow[m_Columns.m_col_nickname]);
			nickname_copy = parseMarkup(nickname_copy);
			label->set_markup("To: " + nickname_copy + " &lt;" + childrow[m_Columns.m_col_status] + "&gt;");
			if (childrow[m_Columns.m_col_status] == "Offline") {
				//updateStatusTextView(text_view, childrow[m_Columns.m_col_nickname], 0);
				//text_view2->get_buffer()->set_text(offline_msg);
				text_view2->set_sensitive(false);
			}
			else {
				//updateStatusTextView(text_view, childrow[m_Columns.m_col_nickname], 1);
				//text_view2->get_buffer()->set_text("");
				text_view2->set_sensitive(true);
			}
		}
	}
	
}

void ConvoWin::updateTextViewInputStyle() {
	
	Gtk::VBox *vbox;
	Gtk::TextView *text_view_input;
	Gtk::ToggleButton *apply_btn;
		
	for(int i = 0; i != convonotebook_->get_n_pages(); i++) {
		vbox = (Gtk::VBox *) (convonotebook_->get_nth_page(i));
		text_view_input = getTextViewInput(vbox);
		apply_btn = getApplyBtn(vbox);
		Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch =
				text_view_input->get_buffer()->get_tag_table()->lookup("inputtag");
		if (app_ptr_->getConfig()->getConfigOptions()->getEnableStyle() == "1" && 
				  apply_btn->get_active()) {
			refTagMatch->property_font() = app_ptr_->getConfig()->getConfigOptions()->getFont();
			refTagMatch->property_foreground() = app_ptr_->getConfig()->getConfigOptions()->getColor();
		}
		else {
			refTagMatch->property_font() = "";
			refTagMatch->property_foreground() = "black";
			apply_btn->set_active(0);
			refTagMatch->property_underline_set() = 0;
			refTagMatch->property_strikethrough_set() = 0;
		}
		apply_btn->set_sensitive(intify(app_ptr_->getConfig()->getConfigOptions()->getEnableStyle()));
		text_view_input->get_buffer()->apply_tag_by_name("inputtag", 
					    text_view_input->get_buffer()->begin(),
							    text_view_input->get_buffer()->end());
			
	}

}

void ConvoWin::updateTextViewInputStyle(Glib::RefPtr<Gtk::TextBuffer> buffer, Gtk::ToggleButton *apply_btn) {
	
	Glib::RefPtr<Gtk::TextBuffer::Tag> refTagMatch =
			buffer->get_tag_table()->lookup("inputtag");
	
	if (app_ptr_->getConfig()->getConfigOptions()->getEnableStyle() == "1" && 
		   apply_btn->get_active()) {
		
		refTagMatch->property_font() = app_ptr_->getConfig()->getConfigOptions()->getFont();
		refTagMatch->property_foreground() = app_ptr_->getConfig()->getConfigOptions()->getColor();
				}
	else {
		refTagMatch->property_font() = "";
		refTagMatch->property_foreground() = "black";
		apply_btn->set_active(0);
		refTagMatch->property_underline_set() = 0;
		refTagMatch->property_strikethrough_set() = 0;
	}
	apply_btn->set_sensitive(intify(app_ptr_->getConfig()->getConfigOptions()->getEnableStyle()));
	buffer->apply_tag_by_name("inputtag", buffer->begin(), buffer->end());

}

void ConvoWin::updateTextViewInputGtkSpell(int detach) {
	
	Gtk::VBox *vbox;
	Gtk::TextView *text_view_input;
	GtkSpell *obj;
	
	for(int i = 0; i != convonotebook_->get_n_pages(); i++) {
		vbox = (Gtk::VBox *) (convonotebook_->get_nth_page(i));
		text_view_input = getTextViewInput(vbox);
		obj = gtkspell_get_from_text_view(text_view_input->gobj());
		
		if (detach && obj)
			gtkspell_detach(obj);
		else if (!detach)
			if (obj)
				gtkspell_detach(obj);
			attachGtkSpellToTextView(text_view_input);
	}
}

void ConvoWin::updateTextViewInputGtkSpellDictionary() {
	
	Gtk::VBox *vbox;
	Gtk::TextView *text_view_input;
	GtkSpell *obj;
	
	for(int i = 0; i != convonotebook_->get_n_pages(); i++) {
		vbox = (Gtk::VBox *) (convonotebook_->get_nth_page(i));
		text_view_input = getTextViewInput(vbox);
		obj = gtkspell_get_from_text_view(text_view_input->gobj());
		
		if (obj)
			setGtkSpellDictionary(obj,
					      app_ptr_->getConfig()->getConfigOptions()->getSpellingDictionary());
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
	Gtk::Label *label_id = getLabelId(vbox);
	Gtk::Label *label_nick = getLabelNick(vbox);
	Gtk::Label *label = getLabel(vbox);
	
	Gtk::TextView *text_view = getTextView(vbox);
	updateTextView(text_view, label_nick->get_text(), message);
	
	BuddyListEntry *buddy_entry = client_->getClient()->getBuddyList()->getBuddyById(intify(label_id->get_text()));
	app_ptr_->getLog()->chatLog(buddy_entry->username, label_nick->get_text(), message);
	
	if (convonotebook_->get_current_page() != convonotebook_->page_num(*vbox)) {
		Gtk::HBox *hbox = (Gtk::HBox *) convonotebook_->get_tab_label(*vbox);	
		Gtk::Label *label = dynamic_cast<Gtk::Label *>((hbox->children().begin())->get_widget());
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
	Gtk::VPaned *vpaned = dynamic_cast<Gtk::VPaned*>((++wi)->get_widget());
	Gtk::EventBox *scrolled_win_eb = dynamic_cast<Gtk::EventBox*>(vpaned->get_child1());
	Gtk::ScrolledWindow *scrolled_win = dynamic_cast<Gtk::ScrolledWindow*>(scrolled_win_eb->get_child());
	Gtk::TextView *text_view = dynamic_cast<Gtk::TextView*>(scrolled_win->get_child());
	Gtk::VBox *entry_vbox = dynamic_cast<Gtk::VBox*>(vpaned->get_child2());
	Gtk::Box_Helpers::BoxList::iterator wi2 = entry_vbox->children().begin();
	Gtk::HBox *buttons_hbox = dynamic_cast<Gtk::HBox*>(wi2->get_widget());
	Gtk::Box_Helpers::BoxList::iterator wi3 = buttons_hbox->children().begin();
	Gtk::Button *font_btn = dynamic_cast<Gtk::Button*>(wi3->get_widget());
	Gtk::Button *color_btn = dynamic_cast<Gtk::Button*>((++wi3)->get_widget());
	Gtk::ToggleButton *underline_btn = dynamic_cast<Gtk::ToggleButton*>((++wi3)->get_widget());
	Gtk::ToggleButton *strikethrough_btn = dynamic_cast<Gtk::ToggleButton*>((++wi3)->get_widget());
	Gtk::ToggleButton *apply_btn = dynamic_cast<Gtk::ToggleButton*>((++wi3)->get_widget());
	Gtk::EventBox *entry_hbox_eb = dynamic_cast<Gtk::EventBox*>((++wi2)->get_widget());
	Gtk::HBox *entry_hbox = dynamic_cast<Gtk::HBox*>(entry_hbox_eb->get_child());
	Gtk::Box_Helpers::BoxList::iterator wi4 = entry_hbox->children().begin();
	Gtk::ScrolledWindow *scrolled_win2 = 
			dynamic_cast<Gtk::ScrolledWindow*>(wi4->get_widget());
	Gtk::TextView *text_view_input = dynamic_cast<Gtk::TextView*>(*(scrolled_win2->get_children().begin()));
	Gtk::EventBox *entry_eb = dynamic_cast<Gtk::EventBox*>((++wi4)->get_widget());
	Gtk::Alignment *entry_alignment = dynamic_cast<Gtk::Alignment*>(entry_eb->get_child());
	Gtk::Button *entry_btn = dynamic_cast<Gtk::Button*>(entry_alignment->get_child());
	
	GtkSpell *spelling_obj = gtkspell_get_from_text_view(text_view_input->gobj());
	if (spelling_obj)
		gtkspell_detach(spelling_obj);
	
	delete text_view_input;
	delete entry_btn;
	delete entry_alignment;
	delete entry_eb;
	delete scrolled_win2;
	delete entry_hbox;
	delete entry_hbox_eb;
	delete font_btn;
	delete color_btn;
	delete underline_btn;
	delete strikethrough_btn;
	delete apply_btn;
	delete buttons_hbox;
	delete entry_vbox;
	delete text_view;
	delete scrolled_win;
	delete scrolled_win_eb;
	delete vpaned;
	delete label;
	delete label_nick;
	delete label_id;
	delete notebook_vbox;
	
}

