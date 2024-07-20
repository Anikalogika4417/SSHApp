#include "User.h"

User::User(const string& _userNmae, const string& _userIP, const string& _status)
	:userName(_userNmae),
	userIP(_userIP),
	userId(GeneratorUuid::generate()),
	status(_status)
{};