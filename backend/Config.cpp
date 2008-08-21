#include <iostream>
#include <string>
#include <fstream>


#include "Config.h"
#include "ConfigOptions.h"

using namespace std;

Config::Config(Glib::ustring config_path, Glib::ustring username, Application *app_ptr) {
	config_path_ = config_path;
	username_ = username;
	app_ptr_ = app_ptr;
	config_options_ = new ConfigOptions(this);
	loadConfigOptions();
}

Config::~Config() {
	delete config_options_;
}

void Config::loadConfigOptions() {
	ifstream config(config_path_.c_str());
	if (!config) {
		config_options_->setAllToDefaults();
	}
	else {
		Glib::ustring tmp = "N";
		config_options_->setNickname(getOption(tmp));
		tmp = "UD";
		config_options_->setUserDirectory(getOption(tmp));
		tmp = "LD";
		config_options_->setLogsDirectory(getOption(tmp));
		tmp = "ES";
		config_options_->setEnableStyle(getOption(tmp));
		tmp = "F";
		config_options_->setFont(getOption(tmp));
		tmp = "C";
		config_options_->setColor(getOption(tmp));
		tmp = "L";
		config_options_->setLogging(getOption(tmp));
		tmp = "LA";
		config_options_->setLogAll(getOption(tmp));
		tmp = "LBS";
		config_options_->setLogBuddyStatus(getOption(tmp));
		tmp = "LMS";
		config_options_->setLogMessageSend(getOption(tmp));
		tmp = "LMR";
		config_options_->setLogMessageReceive(getOption(tmp));
		tmp = "LM";
		config_options_->setLogMain(getOption(tmp));
		tmp = "LE";
		config_options_->setLogError(getOption(tmp));
		tmp = "LSC";
		config_options_->setLogStatusChange(getOption(tmp));
		tmp = "LBR";
		config_options_->setLogBuddyRemove(getOption(tmp));
		tmp = "LIS";
		config_options_->setLogInviteSend(getOption(tmp));
		tmp = "LIR";
		config_options_->setLogInviteReceive(getOption(tmp));
		tmp = "LNC";
		config_options_->setLogNickChange(getOption(tmp));
		tmp = "LBN";
		config_options_->setLogBuddyNickChange(getOption(tmp));
		tmp = "SC";
		config_options_->setSpellChecking(getOption(tmp));
		tmp = "SD";
		config_options_->setSpellingDictionary(getOption(tmp));
	}
}

Glib::ustring Config::getOption(Glib::ustring &oname) {
	ifstream config(config_path_.c_str());
	if (!config) {
			cout << "Cannot open config file for writing" << endl;
			perror("Due to");
	}
	string s;
	Glib::ustring tmp;
	std::vector<Glib::ustring> handle;
	while(getline(config, s)) {
		if (s[0] == '#')
			continue;
		handle = Glib::Regex::split_simple(":", s);
		if (*(handle.begin()) == oname) {
			tmp = *(++(handle.begin()));
			config.close();
			return tmp;
		}
	}
	config.close();
	return "-1";
}

int Config::setOption(Glib::ustring &oname, Glib::ustring &oset, int add = 1) {
	int found = 0;
	string s;
	std::vector<Glib::ustring> handle;
	Glib::ustring buf;
	
	ifstream iconfig(config_path_.c_str());
	if (!iconfig) {
		cout << "No config file exists, creating a new one at: " << config_path_.c_str() << endl;
		ofstream oconfig(config_path_.c_str(), ios::out);
		if (!oconfig) {
			cout << "Cannot open config file for writing" << endl;
			perror("Due to");
		}
		oconfig << oname << ":" << oset << endl;
		oconfig.close();
		return 1;
	}
	
	while(getline(iconfig, s)) {
		if (s[0] == '#')
			buf += s + "\n";
		else {
			handle = Glib::Regex::split_simple(":", s);
			if (*(handle.begin()) == oname) {
				found = 1;
				buf += oname + ':' + oset + "\n";
			}
			else
				buf += s + "\n";
		}
	}
	iconfig.close();
	if (found) {
		ofstream oconfig(config_path_.c_str(), ios::trunc);
		if (!oconfig) {
			cout << "Cannot open config file for writing" << endl;
			perror("Due to");
		}
		oconfig << buf;
		oconfig.close();
	}
	else if (!found && add){
		ofstream oconfig(config_path_.c_str(), ios::trunc);
		if (!oconfig) {
			cout << "Cannot open config file for writing" << endl;
			perror("Due to");
		}
		oconfig << buf << oname << ":" << oset << endl;
		oconfig.close();
	}
	return (found || add);
}
