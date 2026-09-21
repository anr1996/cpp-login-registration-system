#pragma once

#include <string>

namespace validator {

	/* Will return true if the username is 3-30 characters,
	has alphanumeric/underscore/hyphen/period, and starts 
	with and ends with an alphanumerid character 
	*/
	bool isValidUsername(const std::string& username);
	
	/* Will return true if password meets length policy (12-128 chars).
	*/
	bool isValidPassword(const std::string& password);
}
