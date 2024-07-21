#include <iostream>
#include "../ChatApp/Controllers/SshClient.h"

int main() {
    SshClient client("127.0.0.1", 22);
    if (!client.connect()) {
        return 1;
    }

    std::string command = "ls -l";
    std::string result = client.executeCommand(command);

    std::cout << "Command result:\n" << result << std::endl;

    return 0;
}
