#ifndef USER_H
#define USER_H

#include <string>
#include "/ChatApp/ChatAppServer/Services/GeneratorUuid.h"

using namespace std;

struct User {
	const string userId;
	const string userIP;
	string userName;
	string status;

	User(const string& _userNmae, const string& _userIP, const string& _status);
};

#endif //USER_H

