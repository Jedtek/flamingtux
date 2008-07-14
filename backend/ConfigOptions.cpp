#include "ConfigOptions.h"
#include "Config.h"
#include "../Application.h"

ConfigOptions::ConfigOptions(Config *config) : config_(config) {
}

ConfigOptions::~ConfigOptions() {
}

int ConfigOptions::setNickname(Glib::ustring nickname) {
	nickname_ = nickname;

	if (config_->getApplication()->getBuddyListWin())
		config_->getApplication()->getBuddyListWin()->changeNick(nickname);
	
	Glib::ustring tmp("N");
	return config_->setOption(tmp, nickname_, 1);
}

int ConfigOptions::setUserDirectory(Glib::ustring udir) {
	udir_ = udir;
	Glib::ustring tmp("UD");
	return config_->setOption(tmp, udir_, 1);
}

int ConfigOptions::setLogsDirectory(Glib::ustring ldir) {
	ldir_ = ldir;
	Glib::ustring tmp("LD");
	return config_->setOption(tmp, ldir_, 1);
}

int ConfigOptions::setLogging(Glib::ustring logging) {
	logging_ = logging;
	Glib::ustring tmp("L");
	return config_->setOption(tmp, logging_, 1);
}

int ConfigOptions::setLogAll(Glib::ustring la) {
	la_ = la;
	Glib::ustring tmp("LA");
	return config_->setOption(tmp, la_, 1);
}

int ConfigOptions::setLogBuddyStatus(Glib::ustring lbs) {
	lbs_ = lbs;
	Glib::ustring tmp("LBS");
	return config_->setOption(tmp, lbs_, 1);
}

int ConfigOptions::setLogMessageSend(Glib::ustring lms) {
	lms_ = lms;
	Glib::ustring tmp("LMS");
	return config_->setOption(tmp, lms_, 1);
}

int ConfigOptions::setLogMessageReceive(Glib::ustring lmr) {
	lmr_ = lmr;
	Glib::ustring tmp("LMR");
	return config_->setOption(tmp, lmr_, 1);
}

int ConfigOptions::setLogMain(Glib::ustring lm) {
	lm_ = lm;
	Glib::ustring tmp("LM");
	return config_->setOption(tmp, lm_, 1);
}

int ConfigOptions::setLogError(Glib::ustring le) {
	le_ = le;
	Glib::ustring tmp("LE");
	return config_->setOption(tmp, le_, 1);
}

int ConfigOptions::setLogStatusChange(Glib::ustring lsc) {
	lsc_ = lsc;
	Glib::ustring tmp("LSC");
	return config_->setOption(tmp, lsc_, 1);
}

int ConfigOptions::setLogBuddyRemove(Glib::ustring lbr) {
	lbr_ = lbr;
	Glib::ustring tmp("LBR");
	return config_->setOption(tmp, lbr_, 1);
}

int ConfigOptions::setLogInviteSend(Glib::ustring lis) {
	lis_ = lis;
	Glib::ustring tmp("LIS");
	return config_->setOption(tmp, lis_, 1);
}

int ConfigOptions::setLogInviteReceive(Glib::ustring lir) {
	lir_ = lir;
	Glib::ustring tmp("LIR");
	return config_->setOption(tmp, lir_, 1);
}

int ConfigOptions::setLogNickChange(Glib::ustring lnc) {
	lnc_ = lnc;
	Glib::ustring tmp("LNC");
	return config_->setOption(tmp, lnc_, 1);
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
	setLogInviteSend("1");
	setLogInviteReceive("1");
	setLogNickChange("1");
	
}
