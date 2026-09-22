#include "userAccount.hpp"
#include <utility>
#include "validator.hpp"

namespace userSettings {

userAccount::userAccount(std::string u, std::string p, std::string e)
	: username_(std::move(u))
	, password_(std::move(p))
	, email_(std::move(e))
{
	
}

bool userAccount::set_username(std::string u) {
	if (u.empty()) return  false;
	if (!(validator::isValidUsername(u))) return false;  	
	username_ = std::move(u);
	return true;
}

bool userAccount::set_email(std::string e) {
	if (e.empty()) return false;	
	if (!(validator::isValidEmail(e))) return false;  	
	email_ = std::move(e);
	return true;
}

bool userAccount::change_password(std::string_view old_password, std::string new_password){
	if (!check_password(old_password)) return false;
	if (new_password.empty()) return false;
	if (!(validator::isValidPassword(new_password))) return false;
	password_ = std::move(new_password);
	return true;
}

}
 

