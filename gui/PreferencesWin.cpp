#include <vector>
#include "PreferencesWin.h"
#include "../Application.h"
#include "../CommonFunctions.h"

PreferencesWin::PreferencesWin(Glib::RefPtr<Gnome::Glade::Xml> refXml, Application *app) : preferenceswin_(0),
			        app_ptr_(app) {
	// get the preferences window and its required child widgets
	refXml->get_widget("PreferencesWin", preferenceswin_);
	if (!preferenceswin_)
		throw std::runtime_error("Couldn't find PreferencesWin");
	refXml->get_widget("POKBtn", pokbtn_);
	if (!pokbtn_)
		throw std::runtime_error("Couldn't find POKBtn");
	refXml->get_widget("PApplyBtn", papplybtn_);
	if (!papplybtn_)
		throw std::runtime_error("Couldn't find PApplyBtn");
	refXml->get_widget("PCancelBtn", pcancelbtn_);
	if (!pcancelbtn_)
		throw std::runtime_error("Couldn't find PCancelBtn");
	refXml->get_widget("PNicknameEntry", pnicknameentry_);
	if (!pnicknameentry_)
		throw std::runtime_error("Couldn't find PNicknameEntry");
	refXml->get_widget("PUserDirectoryEntry", puserdirectoryentry_);
	if (!puserdirectoryentry_)
		throw std::runtime_error("Couldn't find PUserDirectoryEntry");
	refXml->get_widget("PLoggingDirectoryEntry", ploggingdirectoryentry_);
	if (!ploggingdirectoryentry_)
		throw std::runtime_error("Couldn't find PLoggingDirectoryEntry");
	refXml->get_widget("PLoggingCB", ploggingcb_);
	if (!ploggingcb_)
		throw std::runtime_error("Couldn't find PLoggingCB");
	refXml->get_widget("PLogAllCB", plogallcb_);
	if (!plogallcb_)
		throw std::runtime_error("Couldn't find PLogAllCB");
	refXml->get_widget("PLogMainCB", plogmaincb_);
	if (!plogmaincb_)
		throw std::runtime_error("Couldn't find PLogMainCB");
	refXml->get_widget("PLogErrorCB", plogerrorcb_);
	if (!plogerrorcb_)
		throw std::runtime_error("Couldn't find PLogErrorCB");
	refXml->get_widget("PLogMessageSendCB", plogmessagesendcb_);
	if (!plogmessagesendcb_)
		throw std::runtime_error("Couldn't find PLogMessageSendCB");
	refXml->get_widget("PLogMessageReceiveCB", plogmessagereceivecb_);
	if (!plogmessagereceivecb_)
		throw std::runtime_error("Couldn't find PLogMessageReceiveCB");
	refXml->get_widget("PLogBuddyRemoveCB", plogbuddyremovecb_);
	if (!plogbuddyremovecb_)
		throw std::runtime_error("Couldn't find PLogBuddyRemoveCB");
	refXml->get_widget("PLogBuddyStatusChangeCB", plogbuddystatuschangecb_);
	if (!plogbuddystatuschangecb_)
		throw std::runtime_error("Couldn't find PLogBuddyStatusChangeCB");
	refXml->get_widget("PLogNickChangeCB", plognickchangecb_);
	if (!plognickchangecb_)
		throw std::runtime_error("Couldn't find PLogNickChangeCB");
	refXml->get_widget("PLogBuddyNickChangeCB", plogbuddynickchangecb_);
	if (!plogbuddynickchangecb_)
		throw std::runtime_error("Couldn't find PLogBuddyNickChangeCB");
	refXml->get_widget("PLogInviteSendCB", ploginvitesendcb_);
	if (!ploginvitesendcb_)
		throw std::runtime_error("Couldn't find PLogInviteSendCB");
	refXml->get_widget("PLogInviteReceivedCB", ploginvitereceivedcb_);
	if (!ploginvitereceivedcb_)
		throw std::runtime_error("Couldn't find PLogInviteReceivedCB");
	refXml->get_widget("PLogStatusChangeCB", plogstatuschangecb_);
	if (!plogstatuschangecb_)
		throw std::runtime_error("Couldn't find PLogStatusChangeCB");
	
	pokbtn_->signal_clicked().connect(sigc::mem_fun(*this, &PreferencesWin::onPOKBtnClicked));
	papplybtn_->signal_clicked().connect(sigc::mem_fun(*this, &PreferencesWin::onPApplyBtnClicked));
	pcancelbtn_->signal_clicked().connect(sigc::mem_fun(*this, &PreferencesWin::onPCancelBtnClicked));
// 	pnicknameentry_->signal_changed().connect(sigc::mem_fun(*this, &PreferencesWin::onNicknameEntryChanged));
// 	puserdirectoryentry_->signal_changed().connect(
// 					     sigc::mem_fun(*this, &PreferencesWin::onUserDirectoryEntryChanged));
// 	ploggingdirectoryentry_->signal_changed().connect(
// 			sigc::mem_fun(*this, &PreferencesWin::onLoggingDirectoryEntryChanged));
	plogallcb_->signal_toggled().connect(sigc::mem_fun(*this, &PreferencesWin::onLogAllCBToggled));
	ploggingcb_->signal_toggled().connect(sigc::mem_fun(*this, &PreferencesWin::onLoggingCBToggled));
	plogmaincb_->signal_toggled().connect(sigc::mem_fun(*this, &PreferencesWin::onLogMainCBToggled));
	plogerrorcb_->signal_toggled().connect(sigc::mem_fun(*this, &PreferencesWin::onLogErrorCBToggled));
	plogmessagesendcb_->signal_toggled().connect(
					   sigc::mem_fun(*this, &PreferencesWin::onLogMessageSendCBToggled));
	plogmessagereceivecb_->signal_toggled().connect(
					      sigc::mem_fun(*this, &PreferencesWin::onLogMessageReceiveCBToggled));
	plogbuddyremovecb_->signal_toggled().connect(
					   sigc::mem_fun(*this, &PreferencesWin::onLogBuddyRemoveCBToggled));
	plogbuddystatuschangecb_->signal_toggled().connect(
			sigc::mem_fun(*this, &PreferencesWin::onLogBuddyStatusChangeCBToggled));
	plognickchangecb_->signal_toggled().connect(sigc::mem_fun(*this, &PreferencesWin::onLogNickChangeCBToggled));
	plogbuddynickchangecb_->signal_toggled().connect(
					       sigc::mem_fun(*this, &PreferencesWin::onLogBuddyNickChangeCBToggled));
	ploginvitesendcb_->signal_toggled().connect(sigc::mem_fun(*this, &PreferencesWin::onLogInviteSendCBToggled));
	ploginvitereceivedcb_->signal_toggled().connect(
					     sigc::mem_fun(*this, &PreferencesWin::onLogInviteReceiveCBToggled));
	plogstatuschangecb_->signal_toggled().connect(
					     sigc::mem_fun(*this, &PreferencesWin::onLogStatusChangeCBToggled));
}

PreferencesWin::~PreferencesWin() {
	// nothing here
}

void PreferencesWin::setOptionsFromConfig() {
	pnicknameentry_->set_text(app_ptr_->getConfig()->getConfigOptions()->getNickname());
	puserdirectoryentry_->set_text(app_ptr_->getConfig()->getConfigOptions()->getLogsDirectory());
	ploggingdirectoryentry_->set_text(app_ptr_->getConfig()->getConfigOptions()->getUserDirectory());
	ploggingcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogging()));
	plogallcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogAll()));
	plogmaincb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogMain()));
	plogerrorcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogError()));
	plogmessagesendcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogMessageSend()));
	plogmessagereceivecb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogMessageReceive()));
	plogbuddyremovecb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogBuddyRemove()));
	plogbuddystatuschangecb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogBuddyStatus()));
	plognickchangecb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogNickChange()));
	plogbuddynickchangecb_->set_active(
					   intify(
			app_ptr_->getConfig()->getConfigOptions()->getLogBuddyNickChange()));
	ploginvitesendcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogInviteSend()));
	ploginvitereceivedcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogInviteReceive()));
	plogstatuschangecb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogStatusChange()));
}

void PreferencesWin::onPOKBtnClicked() {
	onNicknameEntryChanged();
	onUserDirectoryEntryChanged();
	onLoggingDirectoryEntryChanged();
	app_ptr_->getConfig()->getConfigOptions()->syncConfigFromConfigOptions();
	preferenceswin_->hide();
}

void PreferencesWin::onPApplyBtnClicked() {
	onNicknameEntryChanged();
	onUserDirectoryEntryChanged();
	onLoggingDirectoryEntryChanged();
	app_ptr_->getConfig()->getConfigOptions()->syncConfigFromConfigOptions();
}

void PreferencesWin::onPCancelBtnClicked() {
	app_ptr_->getConfig()->getConfigOptions()->syncConfigOptionsFromConfig();
	preferenceswin_->hide();
}

void PreferencesWin::onNicknameEntryChanged() {
	app_ptr_->getConfig()->getConfigOptions()->setNickname(pnicknameentry_->get_text(), 0);
}

void PreferencesWin::onUserDirectoryEntryChanged() {
	app_ptr_->getConfig()->getConfigOptions()->setUserDirectory(puserdirectoryentry_->get_text(), 0);
}

void PreferencesWin::onLoggingDirectoryEntryChanged() {
	app_ptr_->getConfig()->getConfigOptions()->setLogsDirectory(ploggingdirectoryentry_->get_text(), 0);
}

void PreferencesWin::onLoggingCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogging(stringify(ploggingcb_->get_active()), 0);
}

void PreferencesWin::onLogAllCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogAll(stringify(plogallcb_->get_active()), 0);
}

void PreferencesWin::onLogMainCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogMain(stringify(plogmaincb_->get_active()), 0);
}

void PreferencesWin::onLogErrorCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogError(stringify(plogerrorcb_->get_active()), 0);
}

void PreferencesWin::onLogMessageSendCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogMessageSend(stringify(plogmessagesendcb_->get_active()), 0);
}

void PreferencesWin::onLogMessageReceiveCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogMessageReceive(stringify(plogmessagereceivecb_->get_active()), 0);
}

void PreferencesWin::onLogBuddyRemoveCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogBuddyRemove(stringify(plogbuddyremovecb_->get_active()), 0);
}

void PreferencesWin::onLogBuddyStatusChangeCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogBuddyStatus(stringify(plogbuddystatuschangecb_->get_active()), 0);
}

void PreferencesWin::onLogBuddyNickChangeCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogBuddyNickChange(
			    stringify(plogbuddynickchangecb_->get_active()), 0);
}

void PreferencesWin::onLogNickChangeCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogNickChange(stringify(plognickchangecb_->get_active()), 0);
}

void PreferencesWin::onLogInviteSendCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogInviteSend(stringify(ploginvitesendcb_->get_active()), 0);
}

void PreferencesWin::onLogInviteReceiveCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogInviteReceive(stringify(ploginvitereceivedcb_->get_active()), 0);
}

void PreferencesWin::onLogStatusChangeCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogStatusChange(stringify(plogstatuschangecb_->get_active()), 0);
}
