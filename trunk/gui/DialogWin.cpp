#include <gtkmm.h>
#include "DialogWin.h"

DialogWin::DialogWin(const Glib::ustring &title,
					 const Glib::ustring &message, 
					 bool use_markup,
					 Gtk::MessageType type,
					 Gtk::ButtonsType buttons,
					 bool modal) {
	dialog = new Gtk::MessageDialog(message, use_markup, type, buttons, modal);
	dialog->set_title(title);
	dialog->show();
}

DialogWin::~DialogWin() {
	delete dialog;
}

