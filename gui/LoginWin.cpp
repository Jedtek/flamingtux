#include <iostream>
#include <stdexcept>
#include <string>

#include "LoginWin.h"
#include "../Application.h"
#include "BuddyListWin.h"

#include <libglademm/xml.h>
#include <gtkmm.h>

using namespace std;
using namespace xfirelib;
//using namespace xfireclient;

LoginWin::LoginWin(Glib::RefPtr<Gnome::Glade::Xml> refXml, Application *app, FireClient *client)
	: loginwin_(0), user_entry_(0), pass_entry_(0), refXml_(refXml) {
	app_ptr_ = app;
	client_ = client;
	
	refXml->get_widget("LoginWin", loginwin_);
	if (!loginwin_)
		throw std::runtime_error("Couldn't find LoginWin");
	refXml->get_widget("user_entry", user_entry_);
	if (!user_entry_)
		throw std::runtime_error("Couldn't find user_entry");
	refXml->get_widget("pass_entry", pass_entry_);
	if (!pass_entry_)
		throw std::runtime_error("Couldn't find pass_entry");
	refXml->get_widget("login_btn", login_btn_);
	if (!login_btn_)
		throw std::runtime_error("Couldn't find login_btn");
	
	login_btn_->signal_clicked().connect(sigc::mem_fun(*this, &LoginWin::login_pressed));
	
	eventThread_ = new BEThread();
	eventThread_->signal_finished().connect(sigc::mem_fun(*this, &LoginWin::on_event_finish));
}

LoginWin::~LoginWin() {
	delete loginwin_;
	delete eventThread_;
}

void LoginWin::login_pressed() {
	// First time we are going to run client, so we have to create a new one
	client_->setUsername(user_entry_->get_text());
	client_->setPassword(pass_entry_->get_text());
	client_->setEventThread(eventThread_);
	client_->run();
}

void LoginWin::eventLoginFailed() {
	Gtk::MessageDialog dialog("Login Failed.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
	dialog.set_secondary_text("Please check username/password");
	dialog.run();
}

void LoginWin::eventLoginSuccess() {
	app_ptr_->getBuddyListWin();
	app_ptr_->hideLoginWin();
}

void LoginWin::on_event_finish() {
	eventThread_->finish();
	switch (eventThread_->getType()) {
		case LOGIN_FAILED: {
			eventLoginFailed();
			break;
		}
		case LOGIN_SUCCESS: {
			eventLoginSuccess();
			break;
		}
		default:
			break;
	}
	eventThread_->setType(NOT_SET);
}
