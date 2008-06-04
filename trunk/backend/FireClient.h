#ifndef ___FIRECLIENT_H___
#define ___FIRECLIENT_H___

#include "common.h"
#include <gtkmm.h>
#include "BackendEvents.h"

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
		private:
			Client *client_;
			string username_;
			string password_;
			int loginStatus_;
			BEThread *eventThread_;
			
			void printBuddyList();
			void launchThread(event_type type);
			
	};
};

using namespace xfireclient;

#endif

