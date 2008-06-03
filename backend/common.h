/* Basic includes */
#include <iostream>
#include <string>
#include <stdexcept>
#include <dlfcn.h>

/* common includes */
#include "xfirelib/client.h"
#include "xfirelib/xfirepacket.h"
#include "xfirelib/loginfailedpacket.h"
#include "xfirelib/loginsuccesspacket.h"
#include "xfirelib/otherloginpacket.h"
#include "xfirelib/messagepacket.h"
#include "xfirelib/sendstatusmessagepacket.h"
#include "xfirelib/sendmessagepacket.h"
#include "xfirelib/invitebuddypacket.h"
#include "xfirelib/sendacceptinvitationpacket.h"
#include "xfirelib/senddenyinvitationpacket.h"
#include "xfirelib/sendremovebuddypacket.h"
#include "xfirelib/sendnickchangepacket.h"
#include "xfirelib/sendgamestatuspacket.h"
#include "xfirelib/sendgamestatus2packet.h"
#include "xfirelib/dummyxfiregameresolver.h"
#include "xfirelib/sendgameserverpacket.h"
#include "xfirelib/recvoldversionpacket.h"
#include "xfirelib/recvstatusmessagepacket.h"
#include "xfirelib/packetlistener.h"
#include "xfirelib/xfirepacket.h"
#include "xfirelib/inviterequestpacket.h"

