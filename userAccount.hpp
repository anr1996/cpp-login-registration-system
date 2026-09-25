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
		std::string email_;
		std::string password_;


	public:
		userAccount(std::string newUser, std::string newEmail, std::string newPass);
	
	// getter functions
	[[nodiscard]] std::string_view get_username() const noexcept {return username_;}
	[[nodiscard]] std::string_view get_email() const noexcept {return email_;}
	
	// setter functions
	bool set_username(std::string user);
	bool set_email(std::string email);

	// password change function which requires the current password.
	bool change_password(std::string_view old_password, std::string new_password);

	// password verification function
	[[nodiscard]] bool check_password(std::string_view attempt) const {
		return attempt == password_;
	}

	

};
}// namespace userSettings


