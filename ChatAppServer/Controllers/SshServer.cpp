#include "SshServer.h"

SshServer::SshServer(const string& _host, int _port) :
	host(_host),
	port(_port),
	session(nullptr),
	sshBind(nullptr)
{};

SshServer::~SshServer() {
	if (session) {
		ssh_disconnect(session);
		ssh_free(session);
	}
	if (sshBind) {
		ssh_bind_free(sshBind);
	}
	ssh_finalize();
};

void SshServer::start() {
	sshBind = ssh_bind_new();
	ssh_bind_options_set(sshBind, SSH_BIND_OPTIONS_BINDADDR, host.c_str());
	ssh_bind_options_set(sshBind, SSH_BIND_OPTIONS_BINDPORT, &port);
	ssh_bind_options_set(sshBind, SSH_BIND_OPTIONS_RSAKEY, "ssh_host_rsa_key");

	if (ssh_bind_listen(sshBind) < 0) {
		cerr << "Error listening to socket: " << ssh_get_error(sshBind) << endl;
		return;
	}

	while (true) {
		session = ssh_new();
		if (ssh_bind_accept(sshBind, session) == SSH_ERROR) {
			cerr << "Error accepting a connection: " << ssh_get_error(sshBind) << endl;
			continue;
		}

		authenticate();
		handleSession();
	}

	authenticate();
	handleSession();
};

void SshServer::authenticate() {
	if (ssh_handle_key_exchange(session)) {
		LOG_ERROR("Error during SSH key exchange");
		cerr << "Error during SSH key exchange: " << ssh_get_error(session) << endl;
		return;
	}

	if (ssh_userauth_password(session, nullptr, "password") == SSH_AUTH_ERROR) {
		cerr << "Error authenticating with password: " << ssh_get_error(session) << endl;
		return;
	}
};

void SshServer::handleSession() {
	ssh_channel channel = ssh_channel_new(session);
	if (ssh_channel_open_session(channel) == SSH_ERROR) {
		cerr << "Error opening channel: " << ssh_get_error(session) << endl;
		ssh_channel_free(channel);
		return;
	}

	channels.push_back(channel);
	char buffer[256];
	int nbytes;
	while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
		buffer[nbytes] = '\0';
		std::string message(buffer);
		handleMessage(channel, message);
	}
	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);
};

void SshServer::handleMessage(ssh_channel channel, const std::string& message) {
	if (message.rfind("cmd:", 0) == 0) {
		std::string result = executeCustomCommand(message.substr(4));
		ssh_channel_write(channel, result.c_str(), result.size());
	}
	else {
		for (auto& ch : channels) {
			if (ch != channel) {
				ssh_channel_write(ch, message.c_str(), message.size());
			}
		}
	}
};

string SshServer::executeCustomCommand(const std::string& command) {
	stringstream result;
	if (command == "hello") {
		result << "Hello from server!\n";
	}
	else if (command == "time") {
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);
		result << "Current server time: " << put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\n";
	}
	else {
		result << "Unknown custom command: " << command << "\n";
	}
	return result.str();
};

