#include <iostream>
#include "../ChatApp/Controllers/SshClient.h"

int main() {
    SshClient client("127.0.0.1", 22);
    if (!client.connect()) {
        return 1;
    }

    std::string message;
    while (true) {
        std::cout << "Enter message or command (prefix with 'cmd:'): ";
        std::getline(std::cin, message);
        if (message == "exit") {
            break;
        }
        client.sendMessage(message);
    }

    return 0;
}
