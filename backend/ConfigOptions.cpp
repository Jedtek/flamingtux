#include "ConfigOptions.h"
#include "Config.h"
#include "../Application.h"

ConfigOptions::ConfigOptions(Config *config) : config_(config) {
}

ConfigOptions::~ConfigOptions() {
}

int ConfigOptions::setNickname(Glib::ustring nickname, int save) {
	nickname_ = nickname;

	if (config_->getApplication()->getBuddyListWin())
		config_->getApplication()->getBuddyListWin()->changeNick(nickname);
	Glib::ustring tmp("N");
	if (save)
		return config_->setOption(tmp, nickname_, 1);
	else return 1;
}

int ConfigOptions::setUserDirectory(Glib::ustring udir, int save) {
	udir_ = udir;
	Glib::ustring tmp("UD");
	if (save)
		return config_->setOption(tmp, udir_, 1);
	else return 1;
}

int ConfigOptions::setLogsDirectory(Glib::ustring ldir, int save) {
	ldir_ = ldir;
	Glib::ustring tmp("LD");
	if (save)
		return config_->setOption(tmp, ldir_, 1);
	else return 1;
}

int ConfigOptions::setLogging(Glib::ustring logging, int save) {
	logging_ = logging;
	Glib::ustring tmp("L");
	if (save)
		return config_->setOption(tmp, logging_, 1);
	else return 1;
}

int ConfigOptions::setLogAll(Glib::ustring la, int save) {
	la_ = la;
	Glib::ustring tmp("LA");
	if (save)
		return config_->setOption(tmp, la_, 1);
	else return 1;
}

int ConfigOptions::setLogBuddyStatus(Glib::ustring lbs, int save) {
	lbs_ = lbs;
	Glib::ustring tmp("LBS");
	if (save)
		return config_->setOption(tmp, lbs_, 1);
	else return 1;
}

int ConfigOptions::setLogMessageSend(Glib::ustring lms, int save) {
	lms_ = lms;
	Glib::ustring tmp("LMS");
	if (save)
		return config_->setOption(tmp, lms_, 1);
	else return 1;
}

int ConfigOptions::setLogMessageReceive(Glib::ustring lmr, int save) {
	lmr_ = lmr;
	Glib::ustring tmp("LMR");
	if (save)
		return config_->setOption(tmp, lmr_, 1);
	else return 1;
}

int ConfigOptions::setLogMain(Glib::ustring lm, int save) {
	lm_ = lm;
	Glib::ustring tmp("LM");
	if (save)
		return config_->setOption(tmp, lm_, 1);
	else return 1;
}

int ConfigOptions::setLogError(Glib::ustring le, int save) {
	le_ = le;
	Glib::ustring tmp("LE");
	if (save)
		return config_->setOption(tmp, le_, 1);
	else return 1;
}

int ConfigOptions::setLogStatusChange(Glib::ustring lsc, int save) {
	lsc_ = lsc;
	Glib::ustring tmp("LSC");
	if (save)
		return config_->setOption(tmp, lsc_, 1);
	else return 1;
}

int ConfigOptions::setLogBuddyRemove(Glib::ustring lbr, int save) {
	lbr_ = lbr;
	Glib::ustring tmp("LBR");
	if (save)
		return config_->setOption(tmp, lbr_, 1);
	else return 1;
}

int ConfigOptions::setLogBuddyNickChange(Glib::ustring lbn, int save) {
	lbn_ = lbn;
	Glib::ustring tmp("LBN");
	if (save)
		return config_->setOption(tmp, lbn_, 1);
	else return 1;
}


int ConfigOptions::setLogInviteSend(Glib::ustring lis, int save) {
	lis_ = lis;
	Glib::ustring tmp("LIS");
	if (save)
		return config_->setOption(tmp, lis_, 1);
	else return 1;
}

int ConfigOptions::setLogInviteReceive(Glib::ustring lir, int save) {
	lir_ = lir;
	Glib::ustring tmp("LIR");
	if (save)
		return config_->setOption(tmp, lir_, 1);
	else return 1;
}

int ConfigOptions::setLogNickChange(Glib::ustring lnc, int save) {
	lnc_ = lnc;
	Glib::ustring tmp("LNC");
	if (save)
		return config_->setOption(tmp, lnc_, 1);
	else return 1;
}

int ConfigOptions::setAllToDefaults() {
	setNickname("");
	string tmp = Glib::get_home_dir() + "/.flamingtux/";
	setUserDirectory(tmp + config_->getUsername());
	setLogsDirectory(tmp + config_->getUsername() + "/logs/");
	setLogging("1");
	setLogAll("1");
	setLogBuddyStatus("1");
	setLogMessageSend("1");
	setLogMessageReceive("1");
	setLogMain("1");
	setLogError("1");
	setLogStatusChange("1");
	setLogBuddyRemove("1");
	setLogBuddyNickChange("1");
	setLogInviteSend("1");
	setLogInviteReceive("1");
	setLogNickChange("1");
	
}

void ConfigOptions::syncConfigFromConfigOptions() {
	setNickname(getNickname());
	setUserDirectory(getUserDirectory());
	setLogsDirectory(getLogsDirectory());
	setLogging(getLogging());
	setLogAll(getLogAll());
	setLogBuddyStatus(getLogBuddyStatus());
	setLogMessageSend(getLogMessageSend());
	setLogMessageReceive(getLogMessageReceive());
	setLogMain(getLogMain());
	setLogError(getLogError());
	setLogStatusChange(getLogStatusChange());
	setLogBuddyRemove(getLogBuddyRemove());
	setLogBuddyNickChange(getLogBuddyNickChange());
	setLogInviteSend(getLogInviteSend());
	setLogInviteReceive(getLogInviteReceive());
	setLogNickChange(getLogNickChange());
}

void ConfigOptions::syncConfigOptionsFromConfig() {
	Glib::ustring tmp("N");
	setNickname(config_->getOption(tmp), 0);
	tmp = "UD";
	setUserDirectory(config_->getOption(tmp), 0);
	tmp = "LD";
	setLogsDirectory(config_->getOption(tmp), 0);
	tmp = "L";
	setLogging(config_->getOption(tmp), 0);
	tmp = "LA";
	setLogAll(config_->getOption(tmp), 0);
	tmp = "LBS";
	setLogBuddyStatus(config_->getOption(tmp), 0);
	tmp = "LMS";
	setLogMessageSend(config_->getOption(tmp), 0);
	tmp = "LMR";
	setLogMessageReceive(config_->getOption(tmp), 0);
	tmp = "LM";
	setLogMain(config_->getOption(tmp), 0);
	tmp = "LE";
	setLogError(config_->getOption(tmp), 0);
	tmp = "LSC";
	setLogStatusChange(config_->getOption(tmp), 0);
	tmp = "LBR";
	setLogBuddyRemove(config_->getOption(tmp), 0);
	tmp = "LBN";
	setLogBuddyNickChange(config_->getOption(tmp), 0);
	tmp = "LIS";
	setLogInviteSend(config_->getOption(tmp), 0);
	tmp = "LIR";
	setLogInviteReceive(config_->getOption(tmp), 0);
	tmp = "LNC";
	setLogNickChange(config_->getOption(tmp), 0);
}

