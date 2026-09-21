#include "accountValidator.hpp"

#include <cctype>

namespace validator {
	
	// If the username length is not within the range (inclusive) of 3-30, return false.	
	bool isValidUsername(const std::string& username) {
		if (!((username.length() >= 3) && (username.length() <= 30))) {
			return false;
		}			

		// if the first character of the username is not a letter or a number return false.
		if (!std::isalnum(static_cast<unsigned char>(username.front()))) {
			return false;
		}
		
		// if any character in the username is not a letter, number, underscore, hyphen, or period
		// then return false. 
		for (char c : username) {
			unsigned char uc = static_cast<unsigned char>(c);
			if (!std::isalnum(uc) && c != '_' && c!= '-' && c!= '.') {
				return false;
			}
		}
		return true;
	}

	bool isValidPassword(const std::string& password) {
		if (!((password.length() >= 12) && (password.length() <= 128))) {
			return false;
		}
		return true;
	}

}
