#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <gtkmm.h>
#include "ConfigOptions.h"

using namespace std;

class Application;

class Config {
	public:
		Config(Glib::ustring config_path, Glib::ustring username, Application *app_ptr);
		~Config();
		Glib::ustring getOption(Glib::ustring &oname);
		int setOption(Glib::ustring &oname, Glib::ustring &oset, int add);
		Glib::ustring getUsername() { return username_; }
		ConfigOptions *getConfigOptions() { return config_options_; }
		Application *getApplication() { return app_ptr_; }
		void loadConfigOptions();
	private:
		Glib::ustring config_path_;
		Glib::ustring username_;
		ConfigOptions *config_options_;
		Application *app_ptr_;
};

#endif
