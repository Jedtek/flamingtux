#include <vector>
#include "PreferencesWin.h"
#include "Application.h"
#include "ConvoWin.h"
#include "CommonFunctions.h"
#include "GtkHelperFunctions.h"

PreferencesWin::PreferencesWin(Glib::RefPtr<Gnome::Glade::Xml> refXml, Application *app) : preferenceswin_(0),
			        app_ptr_(app) {
	// get the preferences window and its required child widgets
	refXml->get_widget("PreferencesWin", preferenceswin_);
	if (!preferenceswin_)
		throw std::runtime_error("Couldn't find PreferencesWin");
	refXml->get_widget("PNotebook", pnotebook_);
	if (!pnotebook_)
		throw std::runtime_error("Couldn't find PNotebook");
	refXml->get_widget("PLogOptionsVB", plogoptionsvb_);
	if (!plogoptionsvb_)
		throw std::runtime_error("Couldn't find PLogOptionsVB");
	refXml->get_widget("POKBtn", pokbtn_);
	if (!pokbtn_)
		throw std::runtime_error("Couldn't find POKBtn");
	refXml->get_widget("PApplyBtn", papplybtn_);
	if (!papplybtn_)
		throw std::runtime_error("Couldn't find PApplyBtn");
	refXml->get_widget("PCancelBtn", pcancelbtn_);
	if (!pcancelbtn_)
		throw std::runtime_error("Couldn't find PCancelBtn");
	refXml->get_widget("PEnableStyleCB", penablestylecb_);
	if (!penablestylecb_)
		throw std::runtime_error("Couldn't find PEnableStyleCB");
	refXml->get_widget("PFontBtn", pfontbtn_);
	if (!pfontbtn_)
		throw std::runtime_error("Couldn't find PFontBtn");
	refXml->get_widget("PColorBtn", pcolorbtn_);
	if (!pcolorbtn_)
		throw std::runtime_error("Couldn't find PColorBtn");
	refXml->get_widget("PNicknameEntry", pnicknameentry_);
	if (!pnicknameentry_)
		throw std::runtime_error("Couldn't find PNicknameEntry");
	refXml->get_widget("PUserDirectoryFCB", puserdirectoryfcb_);
	if (!puserdirectoryfcb_)
		throw std::runtime_error("Couldn't find PUserDirectoryFCB");
	refXml->get_widget("PLoggingDirectoryFCB", ploggingdirectoryfcb_);
	if (!ploggingdirectoryfcb_)
		throw std::runtime_error("Couldn't find PLoggingDirectoryFCB");
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
	refXml->get_widget("PSpellCheckingCB", pspellcheckingcb_);
	if (!pspellcheckingcb_)
		throw std::runtime_error("Couldn't find PSpellCheckingCB");
	refXml->get_widget("PSpellingDictionaryEntry", pspellingdictionaryentry_);
	if (!pspellingdictionaryentry_)
		throw std::runtime_error("Couldn't find PSpellingDictionaryEntry");
	refXml->get_widget("PEnableEventSoundsCB", penableeventsoundscb_);
	if (!penableeventsoundscb_)
		throw std::runtime_error("Couldn't find PEnableEventSoundsCB");
	refXml->get_widget("PESReceivedMessagesCB", pesreceivedmessagescb_);
	if (!pesreceivedmessagescb_)
		throw std::runtime_error("Couldn't find PESReceivedMessagesCB");
	refXml->get_widget("PESBuddyStatusCB", pesbuddystatuscb_);
	if (!pesbuddystatuscb_)
		throw std::runtime_error("Couldn't find PESBuddyStatusCB");
	refXml->get_widget("PESCommandEntry", pescommandentry_);
	if (!pescommandentry_)
		throw std::runtime_error("Couldn't find PESCommandEntry");
	
	preferenceswin_->signal_delete_event().connect(sigc::mem_fun(*this, &PreferencesWin::onDeleteEvent));
	pokbtn_->signal_clicked().connect(sigc::mem_fun(*this, &PreferencesWin::onPOKBtnClicked));
	papplybtn_->signal_clicked().connect(sigc::mem_fun(*this, &PreferencesWin::onPApplyBtnClicked));
	pcancelbtn_->signal_clicked().connect(sigc::mem_fun(*this, &PreferencesWin::onPCancelBtnClicked));
// 	pnicknameentry_->signal_changed().connect(sigc::mem_fun(*this, &PreferencesWin::onNicknameEntryChanged));
 	puserdirectoryfcb_->signal_selection_changed().connect(
 					     sigc::mem_fun(*this,
			&PreferencesWin::onUserDirectoryFCBSelectionChanged));
 	ploggingdirectoryfcb_->signal_selection_changed().connect(
 			sigc::mem_fun(*this, &PreferencesWin::onLoggingDirectoryFCBSelectionChanged));
	penablestylecb_->signal_toggled().connect(sigc::mem_fun(*this, &PreferencesWin::onEnableStyleCBToggled));
	pfontbtn_->signal_font_set().connect(sigc::mem_fun(*this, &PreferencesWin::onFontBtnFontSet));
	pcolorbtn_->signal_color_set().connect(sigc::mem_fun(*this, &PreferencesWin::onColorBtnColorSet));
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
	pspellcheckingcb_->signal_toggled().connect(
					    sigc::mem_fun(*this, &PreferencesWin::onSpellCheckingCBToggled));
	//pspellingdictionaryentry_->signal_changed().connect(
	//				    sigc::mem_fun(*this, &PreferencesWin::onSpellingDictionaryEntryChanged));
	penableeventsoundscb_->signal_toggled().connect(
					      sigc::mem_fun(*this, &PreferencesWin::onEnableEventSoundsCBToggled));
	pesreceivedmessagescb_->signal_toggled().connect(
					      sigc::mem_fun(*this, &PreferencesWin::onESReceivedMessagesCBToggled));
	pesbuddystatuscb_->signal_toggled().connect(
					      sigc::mem_fun(*this, &PreferencesWin::onESBuddyStatusCBToggled));
}

PreferencesWin::~PreferencesWin() {
	// nothing here
}

bool PreferencesWin::onDeleteEvent(GdkEventAny *e) {
	app_ptr_->getConfig()->getConfigOptions()->syncConfigOptionsFromConfig();
	return false;
}

void PreferencesWin::setOptionsFromConfig() {
	pnicknameentry_->set_text(app_ptr_->getConfig()->getConfigOptions()->getNickname());
	puserdirectoryfcb_->set_current_folder(app_ptr_->getConfig()->getConfigOptions()->getUserDirectory());
	ploggingdirectoryfcb_->set_current_folder(app_ptr_->getConfig()->getConfigOptions()->getLogsDirectory());
	penablestylecb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getEnableStyle()));
	pfontbtn_->set_sensitive(penablestylecb_->get_active());
	pcolorbtn_->set_sensitive(penablestylecb_->get_active());
	pfontbtn_->set_font_name(app_ptr_->getConfig()->getConfigOptions()->getFont());
	Gdk::Color color(app_ptr_->getConfig()->getConfigOptions()->getColor());
	pcolorbtn_->set_color(color);
	ploggingcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogging()));
	plogallcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogAll()));
	plogmaincb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogMain()));
	plogerrorcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogError()));
	plogmessagesendcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogMessageSend()));
	plogmessagereceivecb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogMessageReceive()));
	plogbuddyremovecb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogBuddyRemove()));
	plogbuddystatuschangecb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogBuddyStatus()));
	plognickchangecb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogNickChange()));
	plogbuddynickchangecb_->set_active(intify(
					   app_ptr_->getConfig()->getConfigOptions()->getLogBuddyNickChange()));
	ploginvitesendcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogInviteSend()));
	ploginvitereceivedcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogInviteReceive()));
	plogstatuschangecb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getLogStatusChange()));
	pspellcheckingcb_->set_active(intify(app_ptr_->getConfig()->getConfigOptions()->getSpellChecking()));
	pspellingdictionaryentry_->set_text(app_ptr_->getConfig()->getConfigOptions()->getSpellingDictionary());
	penableeventsoundscb_->set_active(intify(
					  app_ptr_->getConfig()->getConfigOptions()->getEnableEventSounds()));
	pescommandentry_->set_sensitive(penableeventsoundscb_->get_active());
	pesreceivedmessagescb_->set_sensitive(penableeventsoundscb_->get_active());
	pesbuddystatuscb_->set_sensitive(penableeventsoundscb_->get_active());
	pesreceivedmessagescb_->set_active(intify(
					  app_ptr_->getConfig()->getConfigOptions()->getESReceivedMessages()));
	pesbuddystatuscb_->set_active(intify(
					  app_ptr_->getConfig()->getConfigOptions()->getESBuddyStatus()));
	pescommandentry_->set_text(app_ptr_->getConfig()->getConfigOptions()->getESCommand());
}

void PreferencesWin::onEntryChanged() {
	onNicknameEntryChanged();
	//onUserDirectoryEntryChanged();
	//onLoggingDirectoryEntryChanged();
	onSpellingDictionaryEntryChanged();
	onESCommandEntryChanged();
}

void PreferencesWin::onPOKBtnClicked() {
	onEntryChanged();
	app_ptr_->getConfig()->getConfigOptions()->syncConfigFromConfigOptions();
	preferenceswin_->hide();
}

void PreferencesWin::onPApplyBtnClicked() {
	onEntryChanged();
	app_ptr_->getConfig()->getConfigOptions()->syncConfigFromConfigOptions();
}

void PreferencesWin::onPCancelBtnClicked() {
	app_ptr_->getConfig()->getConfigOptions()->syncConfigOptionsFromConfig();
	if (app_ptr_->getConvoWin())
		app_ptr_->getConvoWin()->updateTextViewInputStyle();
	preferenceswin_->hide();
}

void PreferencesWin::onNicknameEntryChanged() {
	//if (pnicknameentry_->get_text() != app_ptr_->getConfig()->getConfigOptions()->getNickname())
	app_ptr_->getConfig()->getConfigOptions()->setNickname(pnicknameentry_->get_text(), 0);
}

void PreferencesWin::onUserDirectoryFCBSelectionChanged() {
	app_ptr_->getConfig()->getConfigOptions()->setUserDirectory(puserdirectoryfcb_->get_current_folder(), 0);
}

void PreferencesWin::onLoggingDirectoryFCBSelectionChanged() {
	app_ptr_->getConfig()->getConfigOptions()->setLogsDirectory(ploggingdirectoryfcb_->get_current_folder(), 0);
}

void PreferencesWin::onEnableStyleCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setEnableStyle(stringify(penablestylecb_->get_active()), 0);
	pfontbtn_->set_sensitive(penablestylecb_->get_active());
	pcolorbtn_->set_sensitive(penablestylecb_->get_active());
	if (app_ptr_->getConvoWin())
		app_ptr_->getConvoWin()->updateTextViewInputStyle();
}

void PreferencesWin::onFontBtnFontSet() {
	app_ptr_->getConfig()->getConfigOptions()->setFont(pfontbtn_->get_font_name(), 0);
	if (app_ptr_->getConvoWin())
		app_ptr_->getConvoWin()->updateTextViewInputStyle();
}

void PreferencesWin::onColorBtnColorSet() {
	app_ptr_->getConfig()->getConfigOptions()->setColor(gdkColorToString(pcolorbtn_->get_color()), 0);
	if (app_ptr_->getConvoWin())
		app_ptr_->getConvoWin()->updateTextViewInputStyle();
}

void PreferencesWin::onLoggingCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogging(stringify(ploggingcb_->get_active()), 0);
	plogoptionsvb_->set_sensitive( intify(app_ptr_->getConfig()->getConfigOptions()->getLogging()) &
			(!intify(app_ptr_->getConfig()->getConfigOptions()->getLogAll())) );
	plogallcb_->set_sensitive(ploggingcb_->get_active());
		
}

void PreferencesWin::onLogAllCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setLogAll(stringify(plogallcb_->get_active()), 0);
	plogoptionsvb_->set_sensitive( intify(app_ptr_->getConfig()->getConfigOptions()->getLogging()) &
			(!intify(app_ptr_->getConfig()->getConfigOptions()->getLogAll())) );
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

void PreferencesWin::onSpellCheckingCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setSpellChecking(stringify(pspellcheckingcb_->get_active()), 0);
	pspellingdictionaryentry_->set_sensitive(pspellcheckingcb_->get_active());
	if (app_ptr_->getConvoWin())
		app_ptr_->getConvoWin()->updateTextViewInputGtkSpell(!pspellcheckingcb_->get_active());
}

void PreferencesWin::onSpellingDictionaryEntryChanged() {
	if (pspellingdictionaryentry_->get_text() !=
		    app_ptr_->getConfig()->getConfigOptions()->getSpellingDictionary()) {
		app_ptr_->getConfig()->getConfigOptions()->setSpellingDictionary(
				    pspellingdictionaryentry_->get_text(), 0);
		if (app_ptr_->getConvoWin())
			app_ptr_->getConvoWin()->updateTextViewInputGtkSpellDictionary();
		    }
}

void PreferencesWin::onEnableEventSoundsCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setEnableEventSounds(stringify(penableeventsoundscb_->get_active()), 0);
	pescommandentry_->set_sensitive(penableeventsoundscb_->get_active());
	pesreceivedmessagescb_->set_sensitive(penableeventsoundscb_->get_active());
	pesbuddystatuscb_->set_sensitive(penableeventsoundscb_->get_active());
}

void PreferencesWin::onESReceivedMessagesCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setESReceivedMessages(stringify(pesreceivedmessagescb_->get_active()), 0);
}

void PreferencesWin::onESBuddyStatusCBToggled() {
	app_ptr_->getConfig()->getConfigOptions()->setESBuddyStatus(stringify(pesbuddystatuscb_->get_active()), 0);
}

void PreferencesWin::onESCommandEntryChanged() {
	if (pescommandentry_->get_text() != app_ptr_->getConfig()->getConfigOptions()->getESCommand())
		app_ptr_->getConfig()->getConfigOptions()->setESCommand(pescommandentry_->get_text(), 0);
}
