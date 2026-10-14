#include "userAccount.hpp"
#include "userProfile.hpp"
#include <utility>
#include "validator.hpp"
#include "hasher.hpp"
#include <cctype>


namespace userSettings {

userAccount::userAccount(
			  std::string newUser
			, std::string newEmail
   			, std::string newPass
			, UserProfile newProfile	
			)

	: username_(std::move(newUser))
	, email_(std::move(newEmail))
	, password_(hashPassword(newPass))
	, profile_(std::move(newProfile))


{
	
}


userAccount::userAccount(
			  std::string newUser
			, std::string newEmail
   			, std::string newPass
			, UserProfile newProfile
			, AlreadyHashedTag
			)

	: username_(std::move(newUser))
	, email_(std::move(newEmail))
	, password_(std::move(newPass))
	, profile_(std::move(newProfile))

{
	
}


userAccount userAccount::fromStoredHash(
			  std::string user
			, std::string email
   			, std::string pass
			, UserProfile profile		
			)

{
 return userAccount(
		    std::move(user)
		  , std::move(email)
		  , std::move(pass)
		  , std::move(profile)
		  , AlreadyHashedTag{});

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

bool userAccount::set_profile(UserProfile profile) {
	if (!validator::isValidProfile(profile)) return false;
	profile_ = std::move(profile);
	return true;
}

bool userAccount::change_password(std::string_view old_password, std::string new_password){
	if (!check_password(old_password)) return false;
	if (new_password.empty()) return false;
	if (!(validator::isValidPassword(new_password))) return false;
	password_ = hashPassword(new_password);
	return true;
}

bool userAccount::check_password(std::string_view attempt) const {
	return verifyPassword(std::string(attempt), password_);
}

}


