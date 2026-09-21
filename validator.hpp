#pragma once

#include <string>

namespace validator {
	
	bool isValidUsername(const std::string& username);
	
	bool isValidPassword(const std::string& password);
	
	bool isValidEmail(const std::string& email);
}
