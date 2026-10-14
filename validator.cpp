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

	bool isValidName(const std::string& name){
		static const std::regex pattern(
			"^[a-zA-Z]+([ '-][a-zA-Z]+)*$"
		);
		return name.size() <= 50 && std::regex_match(name, pattern);
	}


	bool isValidPhone(const std::string& phoneNumber){
	static const std::regex pattern(
		"^[0-9]{10}$"
	);
	return std::regex_match(phoneNumber, pattern);
	}

	bool isValidGender(const std::string& gender) {
		return gender == "male" ||
		       gender == "female" ||
		       gender == "non_binary" ||
		       gender == "other";
	}
	
	bool isValidProfile(const userSettings::UserProfile& profile) {
	return isValidName(profile.firstName) &&
	       isValidName(profile.lastName) &&
	       isValidPhone(profile.phoneNumber) &&
	       isValidGender(profile.gender) &&
	       !profile.dateOfBirth.empty() &&
	       !profile.location.empty();	

	}
}
