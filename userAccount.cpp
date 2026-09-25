#include "userAccount.hpp"
#include <utility>
#include "validator.hpp"

namespace userSettings {

userAccount::userAccount(std::string newUser, std::string newEmail, std::string newPass)
	: username_(std::move(newUser))
	, email_(std::move(newEmail))
	, password_(std::move(newPass))
{
	
}

bool userAccount::set_username(std::string user) {
	if (user.empty()) return  false;
	if (!(validator::isValidUsername(user))) return false;  	
	username_ = std::move(user);
	return true;
}

bool userAccount::set_email(std::string email) {
	if (email.empty()) return false;	
	if (!(validator::isValidEmail(email))) return false;  	
	email_ = std::move(email);
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
 

