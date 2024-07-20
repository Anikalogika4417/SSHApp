#include "../ChatAppServer/Controllers/SshServer.h"
#include "../ChatAppServer/Services/Logger.h"

int main() {
    Logger::init();

    SshServer server("0.0.0.0", 22);
    server.start();
    return 0;
}