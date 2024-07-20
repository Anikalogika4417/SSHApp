#ifndef SSHCLIENT_H
#define SSHCLIENT_H

#include <libssh/libssh.h>
#include <string>
#include <iostream>

class SshClient
{
public:
    SshClient(const std::string& host, int port);
    ~SshClient();

    bool connect();
    void sendMessage(const std::string& message);

private:
    std::string host;
    int port;
    ssh_session session;

    bool authenticate();
};
#endif //SSHCLIENT_H
