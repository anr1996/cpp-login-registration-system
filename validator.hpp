#pragma once

#include <string>

namespace validator {
	
	// bool function checks for valid username
	bool isValidUsername(const std::string& username);

	// bool function checks for valid password
	bool isValidPassword(const std::string& password);
		
	// bool function checks for valid email
	bool isValidEmail(const std::string& email);

	// Account creation validator checks all three parameters: username, email, password.	
	bool InitAccountValidator (const std::string& user_, const std::string& email_, const std::string& pass_);
	
} // namespace validator
