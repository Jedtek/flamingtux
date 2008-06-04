#include <iostream>
#include <stdexcept>
#include <string>

#include "Application.h"


#include <libglademm/xml.h>
#include <gtkmm.h>

using namespace std;

Application::Application(string path) 
	: loginWindow(0), buddyListWindow(0), client_(0) {
	refXml_ = Gnome::Glade::Xml::create(path);
	//client_ = new FireClient();
}

Application::~Application() {
	if (client_)
		delete client_;
	if (loginWindow)
		delete loginWindow;
	if (buddyListWindow)
		delete buddyListWindow;
}

// void Application::createNewClient() {
// 	if (client_)
// 		delete client_;
// 	client_ = new FireClient();
// 	loginWindow->setClient(client_);	
// }

// void Application::getLoginWin() {
// 	client_ = new FireClient();
// 	loginWindow = new LoginWin(refXml_, this, client_);
// 	loginWindow->get_window().show();
// }

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

