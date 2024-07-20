#include "SshClient.h"

SshClient::SshClient(const std::string& host, int port)
    : host(host), port(port), session(nullptr) {}

SshClient::~SshClient() {
    if (session) {
        ssh_disconnect(session);
        ssh_free(session);
    }
}

bool SshClient::connect() {
    session = ssh_new();
    if (session == nullptr) {
        std::cerr << "Error creating SSH session." << std::endl;
        return false;
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, host.c_str());
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);

    if (ssh_connect(session) != SSH_OK) {
        std::cerr << "Error connecting to SSH server: " << ssh_get_error(session) << std::endl;
        return false;
    }

    return authenticate();
}

bool SshClient::authenticate() {
    if (ssh_userauth_password(session, nullptr, "password") != SSH_AUTH_SUCCESS) {
        std::cerr << "Authentication failed: " << ssh_get_error(session) << std::endl;
        return false;
    }

    return true;
}

void SshClient::sendMessage(const std::string& message) {
    ssh_channel channel = ssh_channel_new(session);
    if (channel == nullptr) {
        std::cerr << "Error creating SSH channel." << std::endl;
        return;
    }

    if (ssh_channel_open_session(channel) != SSH_OK) {
        std::cerr << "Error opening SSH channel: " << ssh_get_error(session) << std::endl;
        ssh_channel_free(channel);
        return;
    }

    ssh_channel_write(channel, message.c_str(), message.size());

    char buffer[256];
    int nbytes;
    while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
        buffer[nbytes] = '\0';
        std::cout.write(buffer, nbytes);
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
}
