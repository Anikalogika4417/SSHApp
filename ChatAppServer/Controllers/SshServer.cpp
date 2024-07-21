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
		std::cerr << "Error during SSH key exchange: " << ssh_get_error(session) << std::endl;
		return;
	}

	if (ssh_userauth_publickey_auto(session, nullptr, nullptr) != SSH_AUTH_SUCCESS) {
		std::cerr << "Public key authentication failed: " << ssh_get_error(session) << std::endl;
		return;
	}
};

void SshServer::handleSession() {
	ssh_channel channel = ssh_channel_new(session);
	if (ssh_channel_open_session(channel) == SSH_ERROR) {
		std::cerr << "Error opening channel: " << ssh_get_error(session) << std::endl;
		ssh_channel_free(channel);
		return;
	}

	handleMessage(channel);

	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);
};

void SshServer::handleMessage(ssh_channel channel) {
	char buffer[256];
	int nbytes;
	while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
		buffer[nbytes] = '\0';
		std::string command(buffer);
		handleCommand(channel, command);
	}
}

void SshServer::handleCommand(ssh_channel channel, const std::string& command) {
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(_popen(command.c_str(), "r"), _pclose);

	if (!pipe) {
		result = "_popen() failed!";
	}
	else {
		while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
			result += buffer.data();
		}
	}
	ssh_channel_write(channel, result.c_str(), result.size());
}
