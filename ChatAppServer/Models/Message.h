#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

struct Message {

	const string messageText;
	const string messageUserId;
	const string messageDateTime;

	Message(const string& _messageUserId, const string& _messageText);
private:
	string getCurrentTimeAsString();
};

#endif //MESSAGE_H

