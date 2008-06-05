/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is GPL, licensing soon though ;) */
#ifndef __MODELCOLUMNS__H
#define __MODELCOLUMNS__H

#include <libglademm/xml.h>
#include <gtkmm.h>

// Tree model columns
class ModelColumns : public Gtk::TreeModel::ColumnRecord {
	public:
		ModelColumns() { add(m_col_nickname_rendered); add(m_col_nickname); add(m_col_username); add(m_col_status); add(m_col_id); }
		
		Gtk::TreeModelColumn<Glib::ustring> m_col_nickname_rendered;
		Gtk::TreeModelColumn<Glib::ustring> m_col_nickname;
		Gtk::TreeModelColumn<Glib::ustring> m_col_username;
		Gtk::TreeModelColumn<Glib::ustring> m_col_status;
		Gtk::TreeModelColumn<int> m_col_id;
};

#endif

