/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 *  This code is protected by GPLv3, please see LICENSE file for more information */
#ifndef ___FIRECLIENT_H___
#define ___FIRECLIENT_H___

#include "common.h"
#include <gtkmm.h>
#include "BackendEvents.h"
#include <vector>
#include <xfirelib/inviterequestpacket.h>

class Application;

namespace xfireclient {
	using namespace std;
	using namespace xfirelib;
	
	class FireClient : public PacketListener, public sigc::trackable {
		
		public:
			FireClient(Application *app_ptr);
			~FireClient();
			void run();
			void createNewClient();
			void receivedPacket(XFirePacket *packet);
			void disconnect();
			void initSetup();
			Client *getClient() { return client_; }
			void setClient(Client *client) { client_ = client; }
			int getLoginStatus() { return loginStatus_; }
			void setLoginStatus(int x) { loginStatus_ = x; }
			void setEventThread(BEThread *et);
			void setUsername(string username) { username_ = username; }
			void setPassword(string password) { password_ = password; }
			string getUsername() { return username_; }
			string getPassword() { return password_; }
			vector<InviteRequestPacket *> getInviteVector() { return inviteVector; }
			vector<MessagePacket *> getMessageVector() { return messageVector; }
			void clearMessageVector() { messageVector.clear(); }
			void clearInviteVector() { inviteVector.clear(); }
			bool sendMessage(Glib::ustring recepient, Glib::ustring message);
		private:
			Application *app_ptr_;
			Client *client_;
			string username_;
			string password_;
			int loginStatus_;
			BEThread *eventThread_;
			vector<InviteRequestPacket *> inviteVector;
			vector<MessagePacket *> messageVector;
			void printBuddyList();
			void launchThread(event_type type);
			
	};
};

using namespace xfireclient;

#endif

