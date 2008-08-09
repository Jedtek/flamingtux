/* Logging class for flamingtux w00p */
#include <iostream>
#include <string>
#include <fstream>
#include <ctime> 
#include <cerrno>
#include <dirent.h>
#include "Log.h"
#include "../Application.h"
using namespace std;

Glib::ustring& Log::getTimestamp() {
	char buffer[25];
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 25, "%c", timeinfo);
	Glib::ustring *tmp = new Glib::ustring(buffer);
	return *tmp;
}

Log::Log(Glib::ustring logpath, Glib::ustring username, Application *app_ptr) : username_(username) {
	log_path_ = logpath;
	app_ptr_ = app_ptr;
	checkLogPath();
	main_log_ = log_path_ + "/main.log";
	error_log_ = log_path_ + "/error.log";
	chat_log_prefix_ = "_";
	logStart();
}

void Log::checkLogPath() {
	DIR *pdir;
	pdir = opendir(log_path_.c_str());
	if (!pdir) {
		cout << "Log path does not exist." << endl;
		Glib::ustring tmp("Error in Log::checkLogPath(): ");
		writeError(tmp + strerror(errno));
	}
	
}

Log::~Log(){
	logEnd();
}

int Log::logStart() {
	if (!app_ptr_->checkConfigLogOption(app_ptr_->getConfig()->getConfigOptions()->getLogMain()))
		return -1;
	ofstream log(main_log_.c_str(), ios::app);
	if(!log) {
		cout << "Cannot open main log file. Check error log file." << endl;
		Glib::ustring tmp("Error in Log::logStart(): ");
		writeError(tmp + strerror(errno));
		return 1;
	}
	log << "Logging Started At: " << getTimestamp() << endl;
	log.close();
	return 0;
}

int Log::logEnd() {
	if (!app_ptr_->checkConfigLogOption(app_ptr_->getConfig()->getConfigOptions()->getLogMain()))
		return -1;
	ofstream log(main_log_.c_str(), ios::app);
	if(!log) {
		cout << "Cannot open main log file. Check error log file." << endl;
		Glib::ustring tmp("Error in Log::logEnd(): ");
		writeError(tmp + strerror(errno));
		return 1;
	}
	log << "Logging Ended At: " << getTimestamp() << endl;
	log.close();
	return 0;
}

int Log::writeError(Glib::ustring errmsg) {
	if (!app_ptr_->checkConfigLogOption(app_ptr_->getConfig()->getConfigOptions()->getLogError()))
		return -1;
	ofstream log;
	log.open(error_log_.c_str(), ios::app);
	if(!log) {
		cout << "Cannot open error log file" << endl;
		perror("Due to error");
		cout << "Trying to open in home folder" << endl;
		string tmp = Glib::get_home_dir() + "/flamingtux.log";
		log.open(tmp.c_str(), ios::app);
		if (!log) {
			cout << "Cannot open error log file from home folder.";
			perror("Due to error");
			return 1;
		}
	}
	log << getTimestamp() << " *** ERROR: " << errmsg << endl;
	log.close();
	return 0;
}

int Log::writeLog(Glib::ustring msg) {
	if (!app_ptr_->checkConfigLogOption(app_ptr_->getConfig()->getConfigOptions()->getLogMain()))
		return -1;
	ofstream log(main_log_.c_str(), ios::app);
	if(!log) {
		cout << "Cannot open main log file. Check error log file." << endl;
		Glib::ustring tmp("Error in Log::writeLog(): ");
		writeError(tmp + strerror(errno));
		return 1;
	}
	log << getTimestamp() << " " << msg << endl;
	log.close();
	return 0;
}

int Log::logClear() {
	ofstream log(main_log_.c_str(), ios::out);
	if(!log) {
		cout << "Cannot open main log file. Check error log file." << endl;
		Glib::ustring tmp("Error in Log::logClear(): ");
		writeError(tmp + strerror(errno));
		return 1;
	}
	log << getTimestamp() << " New log created" << endl;
	log.close();
	return 0;
}

int Log::chatLog(Glib::ustring username, Glib::ustring nickname, Glib::ustring message) {
	if (!app_ptr_->checkConfigLogOption(app_ptr_->getConfig()->getConfigOptions()->getLogMessageReceive()))
		return -1;
	Glib::ustring tmp_log_file_;
	tmp_log_file_ = log_path_ + '/' + chat_log_prefix_ + username + ".log";
	ofstream log(tmp_log_file_.c_str(), ios::app);
	if(!log) {
		cout << "Cannot open chat log file. Check error log file." << endl;
		Glib::ustring tmp("Error in Log::writeError(): ");
		writeError(tmp + strerror(errno));
		return 1;
	}
	log << getTimestamp() << " " << nickname << " said: " << message << endl;
	log.close();
	return 0;
}

int Log::chatLogSend(Glib::ustring username, Glib::ustring message) {
	if (!app_ptr_->checkConfigLogOption(app_ptr_->getConfig()->getConfigOptions()->getLogMessageSend()))
		return -1;
	Glib::ustring tmp_log_file_;
	tmp_log_file_ = log_path_ + '/' + chat_log_prefix_ + username + ".log";
	ofstream log(tmp_log_file_.c_str(), ios::app);
	if(!log) {
		cout << "Cannot open chat log file. Check error log file." << endl;
		Glib::ustring tmp("Error in chatLogSend(): ");
		writeError(tmp + strerror(errno));
		return 1;	
	}
	log << getTimestamp() << " " << username_ << " said: " << message << endl;
	log.close();
	return 0;
}
