#include "validator.hpp"
#include <regex>

namespace validator {

	bool isValidUsername(const std::string& username){
		static const std::regex pattern(
			"[a-zA-Z0-9][a-zA-Z0-9_.-]{1,28}[a-zA-Z0-9]$"
		);
		return std::regex_match(username, pattern);
	}
	
	bool isValidPassword(const std::string& password){
		static const std::regex pattern(
			"^.{12,128}$"
		);
		return std::regex_match(password, pattern);
	}
	
	bool isValidEmail(const std::string& email){
		static const std::regex pattern(
			"^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"
		);
		return std::regex_match(email, pattern);
	}

}
