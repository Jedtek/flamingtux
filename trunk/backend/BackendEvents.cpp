/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 *  This code is protected by GPLv3, please see LICENSE file for more information */
#include <iostream>

#include <libglademm/xml.h>
#include <gtkmm.h>
#include "BackendEvents.h"

using namespace std;

BEThread::BEThread() 
	: thread_(0), type_(NOT_SET) {
	// connect to the cross-thread signal
	signal_on_event_.connect(sigc::mem_fun(*this, &BEThread::thread_on_event));
}

BEThread::~BEThread() {
	// It is an error if the thread is still running at this point.
	g_return_if_fail(thread_ == 0);		
}

void BEThread::launch(event_type type) {
	type_ = type;
	// create a joinable thread
	thread_ = Glib::Thread::create(sigc::mem_fun(*this, &BEThread::thread_function), true);
}

void BEThread::finish() {
	thread_->join();
	thread_ = 0;
}

sigc::signal<void>& BEThread::signal_finished() {
	return signal_finished_;
}


void BEThread::thread_on_event() {
	cout << "Got event, signalling finish signaley" << endl;
	signal_finished_();
}

void BEThread::thread_function() {
	signal_on_event_();
}

