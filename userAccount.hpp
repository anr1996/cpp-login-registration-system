#pragma once

#include <string>
#include <string_view>

namespace userSettings {

/*
user class for created and verifying the following information:
- username
- email
- password
*/
class userAccount {
	private:
		std::string username_;
		std::string password_;
		std::string email_;


	public:
		userAccount(std::string u, std::string p, std::string e);
	
	// getter functions
	std::string_view username() const noexcept {return username_;}
	std::string_view email() const noexcept {return email_;}
	
	// setter functions
	bool set_username(std::string u);
	bool set_email(std::string e);

	// password change function which requires the current password.
	bool change_password(std::string_view old_password, std::string new_password);

	// password verification function
	bool check_password(std::string_view attempt) const {
		return attempt == password_;
	}

	

};
}
