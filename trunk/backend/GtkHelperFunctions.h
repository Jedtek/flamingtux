/* GtkTextBuffer Helpers header file
None of these functions are mine
Credits to Tim-Philipp Müller
http://bugzilla.gnome.org/show_bug.cgi?id=59390 */

#ifndef ___GTKHELPERFUNCTIONS_H___
#define ___GTKHELPERFUNCTIONS_H___

#include <gtkmm.h>

void gtkTextBufferInsertMarkup(Glib::RefPtr<Gtk::TextBuffer> buffer, Gtk::TextIter iter, const Glib::ustring markup);
void gtkTextBufferInsertMarkupWithTag(Glib::RefPtr<Gtk::TextBuffer> buffer, Gtk::TextIter iter, const Glib::ustring markup, Gtk::TextTag tag);
void gtkTextBufferInsertMarkupWithTag(Glib::RefPtr<Gtk::TextBuffer> buffer, Gtk::TextIter iter, const Glib::ustring markup, Glib::ustring tag_name);
		
Glib::ustring gdkColorToString(Gdk::Color color);

void gtk_text_buffer_insert_markup(GtkTextBuffer *buffer, GtkTextIter *iter, 
				   const gchar *markup);
void gtk_text_buffer_insert_markup_with_tag(GtkTextBuffer *buffer, GtkTextIter *iter, 
					    const gchar *markup, GtkTextTag *tag);
void gtk_text_buffer_set_markup_with_tag(GtkTextBuffer *buffer, const gchar *markup,
					 GtkTextTag *tag);
void gtk_text_buffer_set_markup(GtkTextBuffer *buffer, const gchar *markup);

#endif

