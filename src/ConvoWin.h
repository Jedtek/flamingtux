/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 *  This code is protected by GPLv3, please see LICENSE file for more information */
#ifndef __CONVOWIN__H
#define __CONVOWIN__H

#include <libglademm/xml.h>
#include <gtkmm.h>

#include "GtkSpellProxy.h"
#include "FireClient.h"
#include "BackendEvents.h"

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
		void updateTextViewInputStyle();
		void updateTextViewInputGtkSpell(int detach);
		void updateTextViewInputGtkSpellDictionary();
	private:
		//Glib::ustring offline_msg = "The user is currently offline";
		void updateTextViewInputStyle(Glib::RefPtr<Gtk::TextBuffer> buffer, Gtk::ToggleButton *apply_btn);
		void onCloseMenuItem();
		void updateStatusTextView(Gtk::TextView *text_view, Glib::ustring nickname, int status);
		void closeTab(Gtk::VBox *notebook_vbox);
		void onCloseTabClicked(Gtk::VBox *notebook_vbox);
		virtual bool onDeleteEvent(GdkEventAny *e);
		void onCloseDialogResponseSignal(int response_id, Gtk::MessageDialog *dialog);
		void onWindowStateEvent(GdkEventWindowState* event);
		void onWindowFocus(GdkEventFocus *event);
		void onPageRemoved(Gtk::Widget* page, guint page_num);
		void onPageSwitched(GtkNotebookPage* page, guint page_num);
		void onSendButtonClicked(Gtk::VBox *vbox, Gtk::TreeModel::iterator &iter);
		void updateTextView(Gtk::TextView *text_view, Glib::ustring nickname, Glib::ustring text);
		void onVScrollValueChange(Gtk::ScrolledWindow *scrolled_win);
		void onTextBufferInputChanged(Glib::RefPtr<Gtk::TextBuffer> buffer, Gtk::ToggleButton *apply_btn);
		void onFontBtnClicked();
		void onColorBtnClicked();
		void onForeachButtonsHBox(Gtk::Widget &widget, int sensitive);
		void onApplyBtnToggled(Gtk::TextView *text_view_input,
				       Gtk::HBox *buttons_hbox, Gtk::ToggleButton *apply_btn, 
	   Gtk::ToggleButton *underline_btn, Gtk::ToggleButton *strikethrough_btn);
		void onUnderlineBtnClicked(Gtk::TextView *text_view_input, Gtk::ToggleButton *underline_btn);
		void onStrikethroughBtnClicked(Gtk::TextView *text_view_input, Gtk::ToggleButton *strikethrough_btn);
		Gtk::TextView *getTextViewInput(Gtk::VBox *vbox);
		Gtk::TextView *getTextView(Gtk::VBox *vbox);
		Gtk::Label *getLabelId(Gtk::VBox *vbox);
		Gtk::Label *getLabelNick(Gtk::VBox *vbox);
		Gtk::Label *getLabel(Gtk::VBox *vbox);
		Gtk::ToggleButton *getApplyBtn(Gtk::VBox *vbox);
		void attachGtkSpellToTextView(Gtk::TextView *view);
		void setGtkSpellDictionary(GtkSpell *spell, Glib::ustring lang);
		
		Gtk::Window *convowin_;
		Glib::RefPtr<Gnome::Glade::Xml> refXml_;
		Gtk::Notebook *convonotebook_;
		Gtk::VBox *convowinvbox_;
		Application *app_ptr_;
		FireClient *client_;
		Gtk::ImageMenuItem *close_widget_;
		int windowMinimized;
		int hasFocus;
		int markupCount;
};

#endif

