/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is GPL, licensing soon though ;) */
#ifndef ___BACKENDEVENTS_H___
#define ___BACKENDEVENTS_H___

#include <libglademm/xml.h>
#include <gtkmm.h>

enum event_type { NOT_SET = 0, LOGIN_FAILED, LOGIN_SUCCESS, UPDATE_BUDDY_LIST };

class BEThread : public sigc::trackable {
	public:
		BEThread();
		~BEThread();
		void launch(event_type type);
		void finish();
		
		event_type getType() { return type_; }
 		void setType(event_type type) { type_ = type; }
		
		sigc::signal<void>& signal_finished();
		
	private:
		Glib::Thread *thread_;
		Glib::Dispatcher signal_on_event_;
		sigc::signal<void> signal_finished_;
		
		event_type type_;
		void thread_on_event();
		void thread_function();
};


#endif
