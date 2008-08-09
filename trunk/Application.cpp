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

#include "Application.h"


#include <libglademm/xml.h>
#include <gtkmm.h>

using namespace std;

Application::Application(int argc, char *argv[], string path) 
	: loginWindow(0), buddyListWindow(0), convoWindow(0), client_(0), refXml_(0), path_(path) {
	//refXml_ = Gnome::Glade::Xml::create(path);
	
	checkFlamingTuxDirectory();
	//client_ = new FireClient();
 	//convoWindow = new ConvoWin(refXml_, this, client_);
 	//convoWindow->get_window().hide();
}

Application::~Application() {
 	if (client_)
		delete client_;
	if (loginWindow)
		delete loginWindow;
	if (buddyListWindow)
		delete buddyListWindow;
}

void Application::checkFlamingTuxDirectory() {
	DIR *pdir;
	string tmp(Glib::get_home_dir() + "/.flamingtux/");
	pdir = opendir(tmp.c_str());
	if (!pdir) {
		cout << "FlamingTux directory does not exist. Creating '.flamingtux' in home directory." << endl;
		if (mkdir(tmp.c_str(), S_IRWXU | S_IRWXG | S_IRWXO)) {
			cout << "Could not create FlamingTux directory. No options will be saved and no logging will take place." << endl;
			perror("Error was: ");
		}
	}
}

void Application::run() {
	createNewLoginWin();
}

void Application::createNewLoginWin() {
	refXml_ = Gnome::Glade::Xml::create(path_);
	if (client_)
		delete client_;
	if (loginWindow)
		delete loginWindow;
	client_ = new FireClient(this);
	client_->initSetup();
	loginWindow = new LoginWin(refXml_, this, client_);
	loginWindow->get_window().show();
}

void Application::createBuddyListWin() {
	buddyListWindow = new BuddyListWin(refXml_, this, client_);
	buddyListWindow->get_window().show();
}

void Application::deleteBuddyListWin() {
	delete buddyListWindow;
}

void Application::hideLoginWin() {
	loginWindow->get_window().hide();
}

void Application::showLoginWin() {
	loginWindow->get_window().show();
}

void Application::createNewConvoWin() {
	if (!convoWindow)
		convoWindow = new ConvoWin(refXml_, this, client_);	
}

void Application::deleteConvoWin() {
		if (convoWindow) {
			convoWindow->get_window().hide();
			delete convoWindow;
		}
}

Gtk::Notebook_Helpers::PageIterator Application::appendPageConvoWin(Gtk::TreeModel::iterator &iter) {
	ModelColumns m_Columns;
	
	if (!convoWindow)
		convoWindow = new ConvoWin(refXml_, this, client_);
	convoWindow->get_window().show();
	return convoWindow->appendPage(iter);
}

int Application::checkConfigLogOption(Glib::ustring specific) {
	Glib::ustring l = config_->getConfigOptions()->getLogging();
	if ( (l == "1" && config_->getConfigOptions()->getLogAll() == "1")
		|| (l == "1" && specific == "1") )
		return 1;
	return 0;
}

void Application::quit() {
 	if (loginWindow)
 		delete loginWindow;
 	if (client_)
 		delete client_;
	Gtk::Main::quit();
}