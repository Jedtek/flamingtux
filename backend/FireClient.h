/* FlamingTux 
 * Authors: peacey, jeed890 
 * http://flamingtux.googlecode.com 
 * This code is GPL, licensing soon though ;) */
#ifndef ___FIRECLIENT_H___
#define ___FIRECLIENT_H___

#include "common.h"
#include <gtkmm.h>
#include "BackendEvents.h"
#include <vector>
#include "xfirelib/inviterequestpacket.h"
namespace xfireclient {
	using namespace std;
	using namespace xfirelib;
	
	class FireClient : public PacketListener, public sigc::trackable {
		
		public:
			FireClient(string username, string password);
			FireClient();
			~FireClient();
			void run();
			void receivedPacket(XFirePacket *packet);
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
			bool sendMessage(Glib::ustring recepient, Glib::ustring message);
		private:
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

