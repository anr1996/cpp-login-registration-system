#include "userAccount.hpp"
#include <utility>

namespace userSettings {

userAccount::userAccount(std::string u, std::string p, std::string e)
	: username_(std::move(u))
	, password_(std::move(p))
	, email_(std::move(e))
{}

bool userAccount::set_username(std::string u) {
	if (u.empty()) return  false;
	username_ = std::move(u);
	return true;
}

bool userAccount::set_email(std::string e) {
	if (e.empty()) return false;
	email_ = std::move(e);
	return true;
}

bool userAccount::change_password(std::string_view old_password, std::string new_password){
	if (!check_password(old_password)) return false;
	if (new_password.empty()) return false;
	password_ = std::move(new_password);
	return true;
}

}
 

