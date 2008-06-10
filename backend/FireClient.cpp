/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is GPL, licensing soon though ;) */

// Includes
#include "FireClient.h"
#include "common.h"
#include <vector>
/* Now we begin the declerations of the functions defined in FireClient classey */

namespace xfireclient {
	
	using namespace std;
	using namespace xfirelib;
	
	FireClient::FireClient(string username, string password)
		: username_(username), password_(password) {
		
		client_ = new Client();
		loginStatus_ = 0;
	}
	
	FireClient::FireClient()
	: username_(""), password_("") {
		
		client_ = new Client();
		loginStatus_ = 0;
	}
	
	FireClient::~FireClient() {
		delete client_;
	}
	
	void FireClient::setEventThread(BEThread *et) { eventThread_ = et; }
		
	void FireClient::run() {
		if (username_ == "" || password_ == "")
			cout << "Username or password null!" << endl;
		client_->connect(username_, password_);
		client_->addPacketListener(this);
		/* start the eternal while loop for test purposes*/
		//while (1) { };
	}
	
	void FireClient::launchThread(event_type type) {
		eventThread_->launch(type);	
	}
	void FireClient::receivedPacket(XFirePacket *packet) {
		XFirePacketContent *content = packet->getContent();
		cout << " Received Packet lala: " << content->getPacketId() << endl;
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
					cout << "------------" << endl;
					cout << entry->username << " says:\n"; 
					cout << ((MessagePacket*)content)->getMessage() << endl;
					cout << "------------" << endl;	
					/* now since we got a message lets auto-reply */
// 					SendMessagePacket msg;
// 					msg.init(client_, entry->username, "lool auto-reply!");
// 					client_->send(&msg);
					
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
					}
					else 
						cout << list->getBuddyById(*i)->nick << " <" << list->getBuddyById(*i)->username << ">" << " logged out!" << endl;
				}
 				Glib::signal_idle().connect(
				sigc::bind<1>(sigc::bind_return(sigc::mem_fun(*this, &FireClient::launchThread), false), UPDATE_BUDDY_LIST));
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
						XERROR(( "No such Entry - Got StatusMessage from someone who is not in the buddylist ??\n" ));
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
				client_->disconnect();
				delete client_;
				client_ = 0;
				break;
			}
			case XFIRE_PACKET_INVITE_REQUEST_PACKET: {
				InviteRequestPacket *invite = (InviteRequestPacket*)content;
				InviteRequestPacket *copy_invite = new InviteRequestPacket(*invite);
				cout << "WOOOHOOO WE GOT AN INVITE" << endl;
				cout << "ANN INVIIIIIIIIITEEEEEE" << endl;
				inviteVector.push_back(copy_invite);
				Glib::signal_idle().connect(sigc::bind<1>(sigc::bind_return(sigc::mem_fun(*this, &FireClient::launchThread), false), INVITE_REQUEST)); /* Call to buddylist spawn shit */
				break;
			}
			
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
