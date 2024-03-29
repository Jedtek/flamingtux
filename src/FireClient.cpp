/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 *  This code is protected by GPLv3, please see LICENSE file for more information */
// Includes
#include "Application.h"
#include "FireClient.h"
#include "common.h"
#include <xfirelib/recvremovebuddypacket.h>
#include <xfirelib/recvnicknamechangepacket.h>
#include "CommonFunctions.h"
#include <vector>
/* Now we begin the declerations of the functions defined in FireClient classey */

namespace xfireclient {
	
	using namespace std;
	using namespace xfirelib;
	
	FireClient::FireClient(Application *app_ptr)
	: username_(""), password_(""), client_(0), app_ptr_(app_ptr) {
		
		client_ = new Client();
		loginStatus_ = 0;
	}
	
	FireClient::~FireClient() {
		client_->disconnect();
		if (client_) {
			delete client_;
			client_ = 0;
		}
	}
	
	void FireClient::setEventThread(BEThread *et) { eventThread_ = et; }
		
	void FireClient::createNewClient() {
			if (client_)
				delete client_;
			client_ = new Client();
	}
	
	void FireClient::initSetup() {
		client_->addPacketListener(this);
	}
	
	void FireClient::run() {
		if (username_ == "" || password_ == "")
			cout << "Username or password null!" << endl;
		else {
			client_->connect(username_, password_);
		}
	}
	
	void FireClient::disconnect() {
		client_->disconnect();
	}
	
	void FireClient::launchThread(event_type type) {
		eventThread_->launch(type);	
	}
	void FireClient::receivedPacket(XFirePacket *packet) {
		XFirePacketContent *content = packet->getContent();
		cout << " Received Packet with id: " << content->getPacketId() << endl;
		switch(content->getPacketId()) {
			case XFIRE_LOGIN_FAILED_ID: {
				cout << "Login failed." << endl;
				Glib::signal_idle().connect(
						sigc::bind<1>(sigc::bind_return(sigc::mem_fun(*this, &FireClient::launchThread), false), LOGIN_FAILED));
				loginStatus_ = 0;
 				client_->disconnect();
 				delete client_;
				client_ = 0;
				break;
			}
			case XFIRE_LOGIN_SUCCESS_ID: {
				cout << "Login success!" << endl;
 				Glib::signal_idle().connect(
				sigc::bind<1>(sigc::bind_return(sigc::mem_fun(*this, &FireClient::launchThread), false), LOGIN_SUCCESS));
				loginStatus_ = 1;
				break;
			}
			
			case XFIRE_MESSAGE_ID: {
				cout << "Got Message." << endl;
				if( (( MessagePacket*)content)->getMessageType() == 0){
					MessagePacket *message = (MessagePacket*)content;
					BuddyListEntry *entry = client_->getBuddyList()->getBuddyBySid( ((MessagePacket*)content)->getSid() );
// 					cout << "------------" << endl;
// 					cout << entry->username << " says:\n"; 
// 					cout << ((MessagePacket*)content)->getMessage() << endl;
// 					cout << "------------" << endl;
					
					MessagePacket *copy_message = new MessagePacket(*message);
					messageVector.push_back(copy_message);
					
					Glib::signal_idle().connect(
					sigc::bind<1>(sigc::bind_return(sigc::mem_fun(*this, &FireClient::launchThread), false), GOT_MESSAGE));
				}
				break;
			}
			
			case XFIRE_BUDDYS_ONLINE_ID: {
				BuddyList *list = client_->getBuddyList();
				vector<long>userids = *(((BuddyListOnlinePacket *)content)->userids);
				for (vector<long>::iterator i = userids.begin(); i != userids.end(); ++i) {
					if (list->getBuddyById(*i)->isOnline() == 1) {
						cout << list->getBuddyById(*i)->nick << " <" << list->getBuddyById(*i)->username << ">" << " logged in!" << endl;
						app_ptr_->getLog()->writeLog(list->getBuddyById(*i)->username + " is now online.");
						if (app_ptr_->getConfig()->getConfigOptions()->getEnableEventSounds() == "1")
							playFile(app_ptr_->getConfig()->getConfigOptions()->getESCommand(), app_ptr_->getLogInSoundFilePath());
					}
					else {
						cout << list->getBuddyById(*i)->nick << " <" << list->getBuddyById(*i)->username << ">" << " logged out!" << endl;
						app_ptr_->getLog()->writeLog(list->getBuddyById(*i)->username + " is now offline.");
						if (app_ptr_->getConfig()->getConfigOptions()->getEnableEventSounds() == "1")
							playFile(app_ptr_->getConfig()->getConfigOptions()->getESCommand(), app_ptr_->getLogOutSoundFilePath());
					}
				}
 				Glib::signal_idle().connect(
				sigc::bind<1>(sigc::bind_return(sigc::mem_fun(*this, &FireClient::launchThread), false), UPDATE_BUDDY_LIST));
				break;
			}
			
			case XFIRE_RECV_NICKNAMECHANGE_PACKET_ID: {
				Glib::signal_idle().connect(
						sigc::bind<1>(sigc::bind_return(sigc::mem_fun(*this, &FireClient::launchThread), false), UPDATE_BUDDY_LIST));
				BuddyList *list = client_->getBuddyList();
				if (app_ptr_->getConfig()->getConfigOptions()->getLogBuddyNickChange() == "1")
					app_ptr_->getLog()->writeLog(list->getBuddyById(((RecvNicknameChangePacket *) content)->userId)->username + " changed nickname to: " + ((RecvNicknameChangePacket *) content)->nickname + ".");
				break;
			}
					
			case XFIRE_RECV_OLDVERSION_PACKET_ID: {
				cout << "Our protocol version is too old" << endl;
				break;
			}
			
			case XFIRE_RECV_STATUSMESSAGE_PACKET_ID: {
				RecvStatusMessagePacket *status = (RecvStatusMessagePacket*) content;

				for(uint i = 0 ; i < status->sids->size() ; i++) {
					BuddyListEntry *entry = client_->getBuddyList()->getBuddyBySid( status->sids->at(i) );
					if(entry == NULL) {
// 						XERROR(( "No such Entry - Got StatusMessage from someone who is not in the buddylist ??\n" ));
						return;
					}
					cout << entry->username << " changed status to: " << status->msgs->at(i) << endl;
				}
				Glib::signal_idle().connect(
						sigc::bind<1>(sigc::bind_return(sigc::mem_fun(*this, &FireClient::launchThread), false), UPDATE_BUDDY_LIST));
				break;
			}
			case XFIRE_OTHER_LOGIN: {
				cout << "Someone logged in with our account. Disconnecting..." << endl;
				Glib::signal_idle().connect(sigc::bind<1>(sigc::bind_return(sigc::mem_fun(*this, &FireClient::launchThread), false), OTHER_LOGIN));
				client_->disconnect();
				delete client_;
				client_ = 0;
				break;
			}
			case XFIRE_PACKET_INVITE_REQUEST_PACKET: {
				InviteRequestPacket *invite = (InviteRequestPacket*)content;
				InviteRequestPacket *copy_invite = new InviteRequestPacket(*invite);
				inviteVector.push_back(copy_invite);
				Glib::signal_idle().connect(sigc::bind<1>(sigc::bind_return(sigc::mem_fun(*this, &FireClient::launchThread), false), INVITE_REQUEST)); /* Call to buddylist spawn shit */
				break;
			}
			
			case XFIRE_RECVREMOVEBUDDYPACKET: // fall through
			case XFIRE_BUDDYS_NAMES_ID: {
				Glib::signal_idle().connect(
						sigc::bind<1>(sigc::bind_return(sigc::mem_fun(*this, &FireClient::launchThread), false), UPDATE_BUDDY_LIST));
				cout << "Got buddys name id thingy!" << endl;
// 				printBuddyList();
				
				break;
			}
			
			default:
				cout << "Got nothing..." << endl;
				break;
		}
	}

	void FireClient::printBuddyList() {
		printf("Buddy List: (* marks online users)\n");
		printf("----------------- Buddy List --------------------------------------------------------\n");
		printf("  %20s | %20s | %10s | %20s | %7s | %7s\n","User Name", "Nick", "UserId", "Status Msg" ,"Gameid" ,"Gameid2" );
		vector<BuddyListEntry*> *entries = client_->getBuddyList()->getEntries();
		for(uint i = 0 ; i < entries->size() ; i ++) {
			BuddyListEntry *entry = entries->at(i);
			printf("%1s %20s | %20s | %10ld | %20s | %7ld | %ld\n",
			       (entry->isOnline() ? "*" : ""),
				entry->username.c_str(),
						entry->nick.c_str(),
								entry->userid,
	entry->statusmsg.c_str(),
			       entry->game,
	  entry->game2);
		}
		printf("-------------------------------------------------------------------------------------\n\n");
	}
	
	bool FireClient::sendMessage(Glib::ustring recepient, Glib::ustring message) {
		SendMessagePacket msg;
		msg.init(client_, recepient, message);
		if (client_->send( &msg ) == true)
			return true;
		return false;
	}
};


// int main() {
// 	
// 	using namespace std;
// 	using namespace xfirelib;
// 	using namespace xfireclient_;
// 	
// 	FireClient *testClient = new FireClient("jeed890", "thedude");
// 	testClient->run();
// 	return 0;
// }
