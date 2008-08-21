#ifndef ___PREFERENCESWIN_H___
#define ___PREFERENCESWIN_H___

#include <vector>
#include <libglademm/xml.h>
#include <gtkmm.h>

class Application;
using namespace std;

class PreferencesWin {
	public:
		PreferencesWin(Glib::RefPtr<Gnome::Glade::Xml> refXml, Application *app);
		~PreferencesWin();
		void hide() { preferenceswin_->hide(); }
		void show() { setOptionsFromConfig(); preferenceswin_->show(); }
		void signalFontBtnClicked() { pfontbtn_->clicked(); }
		void signalColorBtnClicked() { pcolorbtn_->clicked(); }
		void setOptionsFromConfig();
	private:
		bool onDeleteEvent(GdkEventAny *e);
		void onPOKBtnClicked();
		void onPCancelBtnClicked();
		void onPApplyBtnClicked();
		void onNicknameEntryChanged();
		void onUserDirectoryEntryChanged();
		void onLoggingDirectoryEntryChanged();
		void onEnableStyleCBToggled();
		void onFontBtnFontSet();
		void onColorBtnColorSet();
		void onLoggingCBToggled();
		void onLogAllCBToggled();
		void onLogErrorCBToggled();
		void onLogMainCBToggled();
		void onLogMessageSendCBToggled();
		void onLogMessageReceiveCBToggled();
		void onLogBuddyRemoveCBToggled();
		void onLogBuddyStatusChangeCBToggled();
		void onLogNickChangeCBToggled();
		void onLogBuddyNickChangeCBToggled();
		void onLogInviteSendCBToggled();
		void onLogInviteReceiveCBToggled();
		void onLogStatusChangeCBToggled();
		void onSpellCheckingCBToggled();
		void onSpellingDictionaryEntryChanged();
		void onEntryChanged();
		
		Application *app_ptr_;
		Gtk::Notebook *pnotebook_;
		Gtk::VBox *plogoptionsvb_;
		Gtk::Window *preferenceswin_;
		Gtk::Button *pokbtn_;
		Gtk::Button *papplybtn_;
		Gtk::Button *pcancelbtn_;
		Gtk::Entry *pnicknameentry_;
		Gtk::Entry *puserdirectoryentry_;
		Gtk::Entry *ploggingdirectoryentry_;
		Gtk::CheckButton *penablestylecb_;
		Gtk::FontButton *pfontbtn_;
		Gtk::ColorButton *pcolorbtn_;
		Gtk::CheckButton *ploggingcb_;
		Gtk::CheckButton *plogallcb_;
		Gtk::CheckButton *plogmaincb_;
		Gtk::CheckButton *plogerrorcb_;
		Gtk::CheckButton *plogmessagesendcb_;
		Gtk::CheckButton *plogmessagereceivecb_;
		Gtk::CheckButton *plogbuddyremovecb_;
		Gtk::CheckButton *plogbuddystatuschangecb_;
		Gtk::CheckButton *plognickchangecb_;
		Gtk::CheckButton *plogbuddynickchangecb_;
		Gtk::CheckButton *ploginvitesendcb_;
		Gtk::CheckButton *ploginvitereceivedcb_;
		Gtk::CheckButton *plogstatuschangecb_;
		Gtk::CheckButton *pspellcheckingcb_;
		Gtk::Entry *pspellingdictionaryentry_;
};

#endif
