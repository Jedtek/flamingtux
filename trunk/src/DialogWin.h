#ifndef ___DIALOGWIN_H___
#define ___DIALOGWIN_H___

class DialogWin {
	public:
		DialogWin(const Glib::ustring &title,
				  const Glib::ustring &message, 
				  bool use_markup,
				  Gtk::MessageType type,
				  Gtk::ButtonsType buttons,
				  bool modal);
		virtual ~DialogWin();
	private:
		Gtk::MessageDialog *dialog;
};

#endif
