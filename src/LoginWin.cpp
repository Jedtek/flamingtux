/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 *  This code is protected by GPLv3, please see LICENSE file for more information */
#include <iostream>
#include <stdexcept>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "LoginWin.h"
#include "Application.h"
#include "BuddyListWin.h"
#include "DialogWin.h"
#include "Log.h"
#include "Config.h"

#include <libglademm/xml.h>
#include <gtkmm.h>

using namespace std;
using namespace xfirelib;
//using namespace xfireclient;

LoginWin::LoginWin(Glib::RefPtr<Gnome::Glade::Xml> refXml, Application *app, FireClient *client)
	: loginwin_(0), user_entry_(0), pass_entry_(0), refXml_(refXml), eventThread_(0), timeoutConnectionId(0) {
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
	refXml->get_widget("progress_bar", progress_bar_);
	if (!progress_bar_)
		throw std::runtime_error("Couldn't find progress_bar");
	
	//progress_bar_->show();
	loginwin_->signal_delete_event().connect(sigc::mem_fun(*this, &LoginWin::onDeleteEvent));
	
	login_btn_->signal_clicked().connect(sigc::mem_fun(*this, &LoginWin::login_pressed));
	eventThread_ = new BEThread();
	eventThread_->signal_finished().connect(sigc::mem_fun(*this, &LoginWin::on_event_finish));
}

LoginWin::~LoginWin() {
	loginwin_->hide();
  	loginwin_ = 0;
	delete eventThread_;
}

bool LoginWin::onDeleteEvent(GdkEventAny *e) {
	Gtk::MessageDialog close_dialog(*this, "Quit FalmingTux", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
	close_dialog.set_secondary_text("Are you sure you want to quit?");
	int d_result = close_dialog.run();
	switch(d_result) {
		case(Gtk::RESPONSE_YES): {
			app_ptr_->quit();
			return false;
			break;
		}
		case(Gtk::RESPONSE_NONE):
		case(Gtk::RESPONSE_NO): {
			/* Do nothing and return true */
			return true;
			break;
		}
	}
}

void LoginWin::login_pressed() {
	// First time we are going to run client, so we have to create a new one
	createUserDirectoryStructure(app_ptr_->getFlamingTuxDirectory(), user_entry_->get_text().lowercase());
	Config *config = new Config(app_ptr_->getFlamingTuxDirectory() + user_entry_->get_text().lowercase() + "/config.cfg", user_entry_->get_text().lowercase(), app_ptr_);
	app_ptr_->setConfig(config);
	Log *log = new Log(config->getConfigOptions()->getLogsDirectory(), user_entry_->get_text(), app_ptr_);
	app_ptr_->setLog(log);
	client_->setUsername(user_entry_->get_text());
	client_->setPassword(pass_entry_->get_text());
	client_->setEventThread(eventThread_);
	client_->run();
	user_entry_->set_sensitive(false);
	pass_entry_->set_sensitive(false);
	login_btn_->set_sensitive(false);
	progress_bar_->show();
	timeoutConnectionId = Glib::signal_timeout().connect(sigc::mem_fun(*this,
			&LoginWin::onProgressBarTimeout), 75 );
}

bool LoginWin::onProgressBarTimeout() {
	progress_bar_->pulse();
	if (timeoutConnectionId)
		return true;
	return false;
}

void LoginWin::eventLoginFailed() {
	timeoutConnectionId = 0;
	progress_bar_->hide();
	loginwin_->set_size_request(-1, -1);
	loginwin_->resize(1, 1);
	user_entry_->set_sensitive(true);
	pass_entry_->set_sensitive(true);
	login_btn_->set_sensitive(true);
	Gtk::MessageDialog dialog("Login Failed", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
	dialog.set_secondary_text("Please check username/password");
	dialog.run();
	//app_ptr_->createNewLoginWin();
	//client_->getClient()->disconnect();
	//delete client_->getClient();
	//app_ptr_->run();
	client_->disconnect();
	app_ptr_->getLog()->writeLog("Login Failed. Invalid username/password.");
	app_ptr_->deleteCreatedClasses();
}

void LoginWin::eventLoginSuccess() {
	timeoutConnectionId = 0;
	progress_bar_->hide();
	loginwin_->set_size_request(-1, -1);
	loginwin_->resize(1, 1);
	user_entry_->set_sensitive(true);
	pass_entry_->set_sensitive(true);
	login_btn_->set_sensitive(true);
	app_ptr_->getLog()->writeLog("Login Success.");
	app_ptr_->createBuddyListWin();
	app_ptr_->hideLoginWin();
	app_ptr_->getConfig()->getConfigOptions()->setNickname((app_ptr_->getConfig()->getConfigOptions()->getNickname()));
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

int LoginWin::createUserDirectoryStructure(Glib::ustring ft_directory, Glib::ustring username) {
	DIR *pdir;
	pdir = opendir(ft_directory.c_str());
	if (!pdir) {
			return -1;
	}
	Glib::ustring tmp(ft_directory + '/' + username);
	Glib::ustring tmp2(tmp + "/logs");
	int result = mkdir(tmp.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) | mkdir(tmp2.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	if (!result)
		return 0;
	return 1;
}
