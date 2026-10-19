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
	
	// bool function checks for valid gender
	bool isValidGender(const std::string& gender);

	// bool function checks for valid date of birth in the format: "YYYY-MM-DD"
	bool isValidDateOfBirth(const std::string& dob);
	
	// Checks that dob is a real date and the person is at least minAge years old today.
	bool isOldEnough(const std::string& dob, int minAge);
	
	/*
	Validates first name, last name, phone number, gender, , minimum age requirement, and date of birth
	for the user profile.
	*/
	bool isValidProfile(const userSettings::UserProfile& profile); 	

} // namespace validator
