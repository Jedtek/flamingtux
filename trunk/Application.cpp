#include <iostream>
#include <stdexcept>
#include <string>

#include "Application.h"


#include <libglademm/xml.h>
#include <gtkmm.h>

using namespace std;

Application::Application(string path) 
	: loginwin_(0), buddylistwin_(0) {
	refXml_ = Gnome::Glade::Xml::create(path);
	client_ = new FireClient();
}

Application::~Application() {
}

void Application::getLoginWin() {
	loginWindow = new LoginWin(refXml_, this, client_);
	loginWindow->get_window().show();
}

void Application::getBuddyListWin() {
// 	cout << "--------------------------------------------" << endl;
// 	cout << client_->getUsername() << endl;
// 	cout << "--------------------------------------------" << endl;
	
	buddyListWindow = new BuddyListWin(refXml_, this, client_);
	buddyListWindow->get_window().show();
}

void Application::hideLoginWin() {
	loginWindow->get_window().hide();
}

