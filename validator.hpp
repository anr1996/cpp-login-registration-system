#pragma once

#include "userProfile.hpp"
#include <string>

namespace validator {
	
	// bool function checks for valid username
	bool isValidUsername(const std::string& username);

	// bool function checks for valid password
	bool isValidPassword(const std::string& password);
		
	// bool function checks for valid email
	bool isValidEmail(const std::string& email);
	
	// bool function checks for valid first and last name
	bool isValidName(const std::string& Name);

	// bool function checks for valid phone number
	bool isValidPhone(const std::string& phoneNumber);

	bool isValidGender(const std::string& gender);
	
	bool isValidProfile(const userSettings::UserProfile& profile);
} // namespace validator
