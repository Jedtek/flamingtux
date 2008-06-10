/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is GPL, licensing soon though ;) */
#include <iostream>
#include <stdexcept>
#include <string>

#include "Application.h"


#include <libglademm/xml.h>
#include <gtkmm.h>

using namespace std;

Application::Application(int argc, char *argv[], string path) 
	: loginWindow(0), buddyListWindow(0), convoWindow(0), client_(0) {
	refXml_ = Gnome::Glade::Xml::create(path);
	
	//client_ = new FireClient();
// 	convoWindow = new ConvoWin(refXml_, this, client_);
// 	convoWindow->get_window().hide();
}

Application::~Application() {
	if (client_)
		delete client_;
	if (loginWindow)
		delete loginWindow;
	if (buddyListWindow)
		delete buddyListWindow;
}

void Application::run() {
	createNewLoginWin();
}

void Application::createNewLoginWin() {
	if (loginWindow)
		delete loginWindow;
	if (client_)
		delete client_;
	client_ = new FireClient();
	loginWindow = new LoginWin(refXml_, this, client_);
	loginWindow->get_window().show();
}

void Application::createBuddyListWin() {
// 	cout << "--------------------------------------------" << endl;
// 	cout << client_->getUsername() << endl;
// 	cout << "--------------------------------------------" << endl;
	
	buddyListWindow = new BuddyListWin(refXml_, this, client_);
	buddyListWindow->get_window().show();
}

void Application::hideLoginWin() {
	loginWindow->get_window().hide();
}

void Application::createNewConvoWin() {
	if (!convoWindow)
		convoWindow = new ConvoWin(refXml_, this, client_);	
}
 
Gtk::Notebook_Helpers::PageIterator Application::appendPageConvoWin(Gtk::TreeModel::iterator &iter) {
	ModelColumns m_Columns;
	
	if (!convoWindow)
		convoWindow = new ConvoWin(refXml_, this, client_);
	convoWindow->get_window().show();
	return convoWindow->appendPage(iter);
}

void Application::quit() {
	Gtk::Main::quit();
}