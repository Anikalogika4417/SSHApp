#include "Message.h"

Message::Message(const string& _messageUserId, const string& _messageText)
	:messageText(_messageUserId),
	messageUserId(_messageText),
	messageDateTime(Message::getCurrentTimeAsString())
{};

string Message::getCurrentTimeAsString() {
	auto now = std::chrono::system_clock::now();
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
	return ss.str();
}
