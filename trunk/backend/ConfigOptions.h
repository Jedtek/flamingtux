#ifndef __CONFIGOPTIONS_H__
#define __CONFIGOPTIONS_H__

#include <glibmm.h>

class Config;

class ConfigOptions {
	public:
		ConfigOptions(Config *config);
		~ConfigOptions();
		int setNickname(Glib::ustring nickname);
		Glib::ustring getNickname() { return nickname_; }
		int setUserDirectory(Glib::ustring udir);
		Glib::ustring getUserDirectory() { return udir_; }
		int setLogsDirectory(Glib::ustring ldir);
		Glib::ustring getLogsDirectory() { return ldir_; }
		int setLogging(Glib::ustring logging);
		Glib::ustring getLogging() { return logging_; }
		int setLogAll(Glib::ustring la);
		Glib::ustring getLogAll() { return la_; }
		int setLogBuddyStatus(Glib::ustring lbs);
		Glib::ustring getLogBuddyStatus() { return lbs_; }
		int setLogMessageSend(Glib::ustring lms);
		Glib::ustring getLogMessageSend() { return lms_; }
		int setLogMessageReceive(Glib::ustring lmr);
		Glib::ustring getLogMessageReceive() { return lmr_; }
		int setLogMain(Glib::ustring lm);
		Glib::ustring getLogMain() { return lm_; }
		int setLogError(Glib::ustring le);
		Glib::ustring getLogError() { return le_; }
		int setLogStatusChange(Glib::ustring lsc);
		Glib::ustring getLogStatusChange() { return lsc_; }
		int setLogBuddyRemove(Glib::ustring lbr);
		Glib::ustring getLogBuddyRemove() { return lbr_; }
		int setLogInviteSend(Glib::ustring lis);
		Glib::ustring getLogInviteSend() { return lis_; }
		int setLogInviteReceive(Glib::ustring lir);
		Glib::ustring getLogInviteReceive() { return lir_; }
		int setLogNickChange(Glib::ustring lnc);
		Glib::ustring getLogNickChange() { return lnc_; }
		int setAllToDefaults();
		
	private:
		Config *config_;
		Glib::ustring nickname_;
		Glib::ustring udir_;
		Glib::ustring ldir_;
		Glib::ustring logging_;
		Glib::ustring la_;
		Glib::ustring lbs_;
		Glib::ustring lms_;
		Glib::ustring lmr_;
		Glib::ustring lm_;
		Glib::ustring le_;
		Glib::ustring lsc_;
		Glib::ustring lbr_;
		Glib::ustring lis_;
		Glib::ustring lir_;
		Glib::ustring lnc_;
};

#endif

