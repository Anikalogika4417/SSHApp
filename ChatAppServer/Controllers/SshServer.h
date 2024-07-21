#ifndef SSHSERVER_H
#define SSHSERVER_H

#include <libssh/server.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <array>
#include <memory>

#include "../ChatApp/ChatAppServer/Services/Logger.h"

using namespace std;

class SshServer
{
public:
	SshServer(const string& _host, int port);
	~SshServer();

	void start();

private:
	string host;
	int port;
	ssh_session session;
	ssh_bind sshBind;


	void handleSession();
	void handleMessage(ssh_channel channel);
	void handleCommand(ssh_channel channel, const std::string& command);
	void authenticate();
};

#endif //CONVERSATIONCONTROLLERSERVER_H

