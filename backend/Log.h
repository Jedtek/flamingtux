#ifndef __LOG__H
#define __LOG__H

#include <gtkmm.h>
#include <string>
#include <ctime>
using namespace std;

class Application;

class Log {
	public:
		Log(Glib::ustring logpath, Glib::ustring username, Application *app_ptr);
		~Log();
		void setUsername(Glib::ustring username) { username_ = username; }
		int writeLog(Glib::ustring msg);
		int writeError(Glib::ustring errmsg);
		int logClear();
		int chatLog(Glib::ustring username, Glib::ustring nickname, Glib::ustring message);
		int chatLogSend(Glib::ustring username, Glib::ustring message);
	private:
		Application *app_ptr_;
		Glib::ustring username_;
		Glib::ustring main_log_;
		Glib::ustring error_log_;
		Glib::ustring chat_log_prefix_;
		Glib::ustring log_path_;
		int logStart();
		int logEnd();
		char *asctime_wn(const struct tm *timeptr);
		char *getTimestamp();
		void checkLogPath();
};

#endif
