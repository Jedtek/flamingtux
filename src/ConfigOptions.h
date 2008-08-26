#ifndef __CONFIGOPTIONS_H__
#define __CONFIGOPTIONS_H__

#include <glibmm.h>

class Config;

class ConfigOptions {
	public:
		ConfigOptions(Config *config);
		~ConfigOptions();
		int setNickname(Glib::ustring nickname, int save = 1);
		Glib::ustring getNickname() { return nickname_; }
		int setUserDirectory(Glib::ustring udir, int save = 1);
		Glib::ustring getUserDirectory() { return udir_; }
		int setLogsDirectory(Glib::ustring ldir, int save = 1);
		Glib::ustring getLogsDirectory() { return ldir_; }
		int setEnableStyle(Glib::ustring es, int save = 1);
		Glib::ustring getEnableStyle() { return es_; }
		int setFont(Glib::ustring font, int save = 1);
		Glib::ustring getFont() { return font_; }
		int setColor(Glib::ustring color, int save = 1);
		Glib::ustring getColor() { return color_; }
		int setLogging(Glib::ustring logging, int save = 1);
		Glib::ustring getLogging() { return logging_; }
		int setLogAll(Glib::ustring la, int save = 1);
		Glib::ustring getLogAll() { return la_; }
		int setLogBuddyStatus(Glib::ustring lbs, int save = 1);
		Glib::ustring getLogBuddyStatus() { return lbs_; }
		int setLogMessageSend(Glib::ustring lms, int save = 1);
		Glib::ustring getLogMessageSend() { return lms_; }
		int setLogMessageReceive(Glib::ustring lmr, int save = 1);
		Glib::ustring getLogMessageReceive() { return lmr_; }
		int setLogMain(Glib::ustring lm, int save = 1);
		Glib::ustring getLogMain() { return lm_; }
		int setLogError(Glib::ustring le, int save = 1);
		Glib::ustring getLogError() { return le_; }
		int setLogStatusChange(Glib::ustring lsc, int save = 1);
		Glib::ustring getLogStatusChange() { return lsc_; }
		int setLogBuddyRemove(Glib::ustring lbr, int save = 1);
		Glib::ustring getLogBuddyRemove() { return lbr_; }
		int setLogBuddyNickChange(Glib::ustring lbn, int save = 1);
		Glib::ustring getLogBuddyNickChange() { return lbn_; }
		int setLogInviteSend(Glib::ustring lis, int save = 1);
		Glib::ustring getLogInviteSend() { return lis_; }
		int setLogInviteReceive(Glib::ustring lir, int save = 1);
		Glib::ustring getLogInviteReceive() { return lir_; }
		int setLogNickChange(Glib::ustring lnc, int save = 1);
		Glib::ustring getLogNickChange() { return lnc_; }
		int setSpellChecking(Glib::ustring sc, int save = 1);
		Glib::ustring getSpellChecking() { return sc_; }
		int setSpellingDictionary(Glib::ustring sd, int save = 1);
		Glib::ustring getSpellingDictionary() { return sd_; }
		int setEnableEventSounds(Glib::ustring ees, int save = 1);
		Glib::ustring getEnableEventSounds() { return ees_; }
		int setESReceivedMessages(Glib::ustring esr, int save = 1);
		Glib::ustring getESReceivedMessages() { return esr_; }
		int setESBuddyStatus(Glib::ustring esb, int save = 1);
		Glib::ustring getESBuddyStatus() { return esb_; }
		int setESCommand(Glib::ustring esc, int save = 1);
		Glib::ustring getESCommand() { return esc_; }
		int setAllToDefaults();
		void syncConfigFromConfigOptions();
		void syncConfigOptionsFromConfig();
	private:
		Config *config_;
		Glib::ustring nickname_;
		Glib::ustring udir_;
		Glib::ustring ldir_;
		Glib::ustring font_;
		Glib::ustring color_;
		Glib::ustring logging_;
		Glib::ustring la_;
		Glib::ustring lbs_;
		Glib::ustring lms_;
		Glib::ustring lmr_;
		Glib::ustring lm_;
		Glib::ustring le_;
		Glib::ustring lsc_;
		Glib::ustring lbr_;
		Glib::ustring lbn_;
		Glib::ustring lis_;
		Glib::ustring lir_;
		Glib::ustring lnc_;
		Glib::ustring es_;
		Glib::ustring sc_;
		Glib::ustring sd_;
		Glib::ustring ees_;
		Glib::ustring esr_;
		Glib::ustring esb_;
		Glib::ustring esc_;
};

#endif

